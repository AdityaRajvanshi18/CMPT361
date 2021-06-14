
#include "include/Angel.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "global.h"
#include "sphere.h"
#include "image_util.h"
#include "scene.h"


int winWidth = WIN_WIDTH;
int winHeight = WIN_HEIGHT;

GLfloat frame[WIN_HEIGHT][WIN_WIDTH][3];   

float image_width = IMAGE_WIDTH;
float image_height = (float(WIN_HEIGHT) / float(WIN_WIDTH)) * IMAGE_WIDTH;

RGB_float background_clr; // background color
RGB_float null_clr = {0.0, 0.0, 0.0};   // NULL color

Point eye_pos = {0.0, 0.0, 0.0};  // eye position
float image_plane = -1.5;         // image plane position

Spheres *scene = NULL;

Point light1;
float light1_ambient[3];
float light1_diffuse[3];
float light1_specular[3];

float global_ambient[3];

float decay_a;
float decay_b;
float decay_c;

int step_max = 5;

int shadow_on = 1;
int reflections_on = 1;
int chessboard_on = 1;
int refractions_on = 1;
int debugger = 0;
int loopertest = 0;

//Checkerboard plane definition using a point and a normal vector

Point chess_point = {0, 3, 0};
Vector chess_board_normal = {0, 15, 4}; //MUST BE NORMALIZED BEFORE USE

float cb_ambient_black[3] = {0, 0, 0};    
float cb_ambient_white[3] = {1, 1, 1};    
float cb_diffuse_black[3] = {0,0,0};
float cb_diffuse_white[3] = {1,1,1};
float cb_specular_black[3] = {0,0,0};
float cb_specular_white[3] = {1,1,1};		
float cb_shineness = 1;
float cb_reflectance = 0.5;


// OpenGL
const int NumPoints = 6;

void init()
{
	// Vertices of a square
	double ext = 1.0;
	vec4 points[NumPoints] = {
		vec4( -ext, -ext,  0, 1.0 ), //v1
		vec4(  ext, -ext,  0, 1.0 ), //v2
		vec4( -ext,  ext,  0, 1.0 ), //v3	
		vec4( -ext,  ext,  0, 1.0 ), //v3	
		vec4(  ext, -ext,  0, 1.0 ), //v2
		vec4(  ext,  ext,  0, 1.0 )  //v4
	};

	// Texture coordinates
	vec2 tex_coords[NumPoints] = {
		vec2( 0.0, 0.0 ),
		vec2( 1.0, 0.0 ),
		vec2( 0.0, 1.0 ),
		vec2( 0.0, 1.0 ),
		vec2( 1.0, 0.0 ),
		vec2( 1.0, 1.0 )
	};

	// Initialize texture objects
	GLuint texture;
	glGenTextures( 1, &texture );

	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0,
		GL_RGB, GL_FLOAT, frame );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glActiveTexture( GL_TEXTURE0 );

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(tex_coords), NULL, GL_STATIC_DRAW );
	GLintptr offset = 0;
	glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(points), points );
	offset += sizeof(points);
	glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(tex_coords), tex_coords );

	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );

	// set up vertex arrays
	offset = 0;
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(offset) );

	offset += sizeof(points);
	GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" ); 
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(offset) );

	glUniform1i( glGetUniformLocation(program, "texture"), 0 );

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glDrawArrays( GL_TRIANGLES, 0, NumPoints );

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':case 'Q':
		free(scene);
		exit(0);
		break;
	case 's':case 'S':
		saveImage();
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main( int argc, char **argv )
{
	// Parse the arguments
	if (argc < 1) {
		printf("Please specify which scene to render, use:\n");
		printf("./rayT [ 1 | 2 | 3 | 4 ]\n");
		return -1;
	}
	
	if (strcmp(argv[1], "1") == 0) { 
		scene1();
	} 
	else if (strcmp(argv[1], "2") == 0) {
		scene2();
	}
	else if (strcmp(argv[1], "3") == 0) { 
		scene3();
	} 
	else if (strcmp(argv[1], "4") == 0) { 
		scene4();
	}
	else{
		printf("Something went wrong. Please specify which scene to render, use:\n");
		printf("./rayT [ 1 | 2 | 3 | 4 ]\n");
	}


	printf("Rendering scene, please wait\n");
	render();

	histogramNormalization();

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );
	glutCreateWindow( "RayTracer" );
	glewInit();
	init();

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMainLoop();
	return 0;
}

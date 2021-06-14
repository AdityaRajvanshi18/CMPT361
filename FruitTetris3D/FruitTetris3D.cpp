/*
	FruitTetris3D
	Aditya Rajvanshi
*/

#include "include/Angel.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <cstring>
#include <assert.h>
#include <math.h>
#include <unistd.h>

using namespace std;


// xsize and ysize are the window sizes
GLfloat xsize = 720; 
GLfloat ysize = 720;
GLfloat zsize = 720;

// current tile
vec2 tile[4]; // An array of 4 2d vectors
vec2 tilepos = vec2(5, 19); // Current tile position where (0,0) is the bottom left corner

// Arrays storing all rotations of each tile shape
vec2 allRotationsLshape[4][4] = 
	{{vec2(0,0), vec2(-1,0), vec2(1, 0), vec2(-1,-1)},
	{vec2(0,0), vec2(0,-1), vec2(0,1), vec2(1, -1)},     
	{vec2(0,0), vec2(1,0), vec2(-1, 0), vec2(1,  1)},  
	{vec2(0,0), vec2(0,1), vec2(0, -1), vec2(-1, 1)}};

vec2 allRotationsTshape[4][4] =
	{{vec2(0,0), vec2(-1,0), vec2(1,0), vec2(0,-1)},
	{vec2(0,0), vec2(0,-1), vec2(0,1), vec2(1,0)},
	{vec2(0,0), vec2(1,0), vec2(-1,0), vec2(0,1)},
	{vec2(0,0), vec2(0,1), vec2(0,-1), vec2(-1,0)}};

vec2 allRotationsSshape[4][4] =
	{{vec2(0,-1), vec2(1,0), vec2(0,0), vec2(-1,-1)},
	{vec2(1,0), vec2(0,1), vec2(0,0), vec2(1,-1)},
	{vec2(0,0), vec2(-1,-1), vec2(0,-1), vec2(1,0)},
	{vec2(0,0), vec2(1,-1), vec2(1,0), vec2(0,1)}};

vec2 allRotationsIshape[4][4] =
	{{vec2(0,0), vec2(-1,0), vec2(1,0), vec2(-2,0)},
	{vec2(0,0), vec2(0,-1), vec2(0,1), vec2(0,-2)},
	{vec2(-1,0), vec2(0,0), vec2(-2,0), vec2(1,0)},
	{vec2(0,-1), vec2(0,0), vec2(0,-2), vec2(0,1)}};

vec2 allRotationsMirroredLshape[4][4] = 
	{{vec2(0,0), vec2(-1,0), vec2(1, 0), vec2(-1,1)},
	{vec2(0,0), vec2(0,-1), vec2(0,1), vec2(-1, -1)},     
	{vec2(0,0), vec2(1,0), vec2(-1, 0), vec2(1,  -1)},  
	{vec2(0,0), vec2(0,1), vec2(0, -1), vec2(1, 1)}};

vec2 allRotationsZshape[4][4] =
	{{vec2(0,0), vec2(-1,0), vec2(0,-1), vec2(1,-1)},
	{vec2(0,0), vec2(0,1), vec2(-1,0), vec2(-1,-1)},
	{vec2(0,0), vec2(-1,0), vec2(0,-1), vec2(1,-1)},
	{vec2(0,0), vec2(0,1), vec2(-1,0), vec2(-1,-1)}};

vec2 shapes[6][4][4] =
	{allRotationsLshape,
	allRotationsTshape,
	allRotationsSshape,
	allRotationsIshape,
	allRotationsMirroredLshape,
	allRotationsZshape};

enum orientation{
	DOWN,
	RIGHT,
	UP,
	LEFT
};

enum tileTypes{
	L,
	T,
	S,
	I,
	ML,
	Z
};

enum colors{
	GREEN,
	BLUE,
	RED,
	ORANGE,
	YELLOW
};


int currentTileType;
int currentTileOrientation;

// colors
vec4 darkgrey 	= 	vec4(0.5, 0.5, 0.5, 0.5); 
vec4 grey 	= 	vec4(0.8, 0.8, 0.8, 1.0);
vec4 green 	= 	vec4(0.0, 1.0, 0.0, 1.0); 
vec4 blue 	= 	vec4(0.0, 0.0, 1.0, 1.0); 
vec4 red 	=	vec4(1.0, 0.0, 0.0, 1.0); 
vec4 orange 	= 	vec4(1.0, 0.5, 0.0, 1.0);
vec4 yellow 	= 	vec4(1.0, 1.0, 0.0, 1.0); 
vec4 white  	= 	vec4(1.0, 1.0, 1.0, 1.0);
vec4 black  	= 	vec4(0.0, 0.0, 0.0, 0.0);

vec4 colors[5] = {green, blue, red, orange, yellow}; 
 
//board[x][y] tells us which cells are occupied
bool board[10][20];
bool isDropped = false;

int boardCellColors[10][20]; //Represents the color being used in cell at position (x,y)

/*Board is 10x20 with 6 vertices (2 triangles) per square so 1200 points to color
All spaces will be black to start. As tiles are place, sets of 6 vertices will be 
changed to the right color in boardColors before updating VBOs */
vec4 boardColors[1200];
vec4 newColors[24*6];
vec4 greycolours[24*6];

// location of vertex attributes in the shader program
GLuint vPosition;
GLuint vColor;

// locations of uniform variables in shader program
GLuint locxsize;
GLuint locysize;
GLuint loczsize;

// VAO and VBO
GLuint VAOs[9]; // One VAO for the grid lines, the front, the current tile, the back, the left, the right, the bottom, the top, and the robot arm
GLuint VBOs[18]; // Two Vertex Buffer Objects for each VAO (vertex positions and colours)

bool moveTile(vec2 direction);
bool isLegalPos(vec2 curTilePos);

//Robot rotation
enum { Yaxis = 0, Xaxis = 1, Zaxis = 2, NumAxes = 4 };
int Axis = Xaxis;

GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 }; //-30.0
GLuint theta;

GLfloat bound = 30;
GLfloat rate = 0.0;
GLfloat Xrate = -3;
GLfloat Zrate = -3;


//Single cube
const int NumVertices = 36; //(6 sides)(2 triangles per side)(3 vertices per triangle)

vec4 c_points[NumVertices]; //All vertices on a cube
vec4 c_colors[NumVertices]; //One color per vertex

GLfloat c_a = 0.0;
GLfloat c_b = 1.0;

vec4 vertices[8] = { //-0.5 --> 0, 0.5 --> 60.0
    vec4( c_a, c_a, c_b, 1.0 ),
    vec4( c_a, c_b, c_b, 1.0 ),
    vec4( c_b, c_b, c_b, 1.0 ),
    vec4( c_b, c_a, c_b, 1.0 ),
    vec4( c_a, c_a, c_a, 1.0 ),
    vec4( c_a, c_b, c_a, 1.0 ),
    vec4( c_b, c_b, c_a, 1.0 ),
    vec4( c_b, c_a, c_a, 1.0 )
};

#define ROBOT_BASE_HEIGHT 33.0
#define ROBOT_BASE_WIDTH 66.0
#define ROBOT_UPPER_ARM_HEIGHT 369.0
#define ROBOT_LOWER_ARM_HEIGHT 369.0
#define ROBOT_UPPER_ARM_WIDTH  20.0
#define ROBOT_LOWER_ARM_WIDTH  40.0

enum {
    Base,
    LowerArm,
    UpperArm,
    NumJointAngles
};

GLfloat phiAngle[NumJointAngles] = {
0.0, 	//Base
10.0, 	//LowerArm
-60.0 	//UpperArm
};

GLint angle = Base; //Global variable to choose which angle to change

int Index = 0;
void
quad( int a, int b, int c, int d ){
    c_colors[Index] = grey; c_points[Index] = vertices[a]; Index++;
    c_colors[Index] = grey; c_points[Index] = vertices[b]; Index++;
    c_colors[Index] = grey; c_points[Index] = vertices[c]; Index++;
    c_colors[Index] = grey; c_points[Index] = vertices[a]; Index++;
    c_colors[Index] = grey; c_points[Index] = vertices[c]; Index++;
    c_colors[Index] = grey; c_points[Index] = vertices[d]; Index++;
}

void
cube( void ){
	Index = 0;
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}
//cube end


//NEEDS WORK HERE
class MatrixStack {
	int _index;
	int _size;
	mat4* _matrices;

public:
	MatrixStack( int numMatrices = 32):_index(0), _size(numMatrices)
	{ _matrices = new mat4[numMatrices]; }

	~MatrixStack()
	{ delete[]_matrices; }

	mat4& push(const mat4& m ){
		assert( _index +1 < _size);
		_matrices[_index++] = m;
	}

	mat4& pop( void ) {
		assert( _index - 1 >= 0);
		_index--;
		return _matrices[_index];
	}
};

MatrixStack mystack;
mat4 modelView, cameraView, projectionView;
GLuint ModelView;
mat4 Id = mat4(1.0);

int random(int bound){
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(0,bound);
	int result = dist(gen);
	return result;
}

vec2 getTilePosition(){
	vec2 result;
	result.y += ROBOT_BASE_HEIGHT;
	result.x += ROBOT_LOWER_ARM_HEIGHT * -sin(M_PI/180.0 * phiAngle[LowerArm]);
	result.y += ROBOT_LOWER_ARM_HEIGHT * cos(M_PI/180.0 * phiAngle[LowerArm]);
	result.x += ROBOT_UPPER_ARM_HEIGHT * -cos(M_PI/180.0 * (90.0 - phiAngle[LowerArm] - phiAngle[UpperArm]));
	result.y += ROBOT_UPPER_ARM_HEIGHT *  sin(M_PI/180.0 * (90.0 - phiAngle[LowerArm] - phiAngle[UpperArm]));

	double val;
	double ceilVal;
	double floorVal;
	for(int i = 0; i<2; i++){
		val = result[i]/33.0;
		ceilVal = ceil(val);
		floorVal = floor(val);
		if((ceilVal - val) < (val - floorVal)) result[i] = ceilVal;
		else result[i] = floorVal;
	}
	return result;
}
// update VBO with the position data everytime a tile is moved or created
void tileUpdate(){
	// Bind the VBO containing current tile vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]); 

	// For each of the 4 'cells' of the tile,
	for (int i = 0; i < 4; i++) {
		// Calculate the grid coordinates of the cell
		GLfloat x = tilepos.x + tile[i].x; 
		GLfloat y = tilepos.y + tile[i].y;

		// Create the 4 corners of the square
		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), 32, 1); 
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), 32, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), 32, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), 32, 1);

		vec4 p5 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), 1, 1);
		vec4 p6 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), 1, 1);
		vec4 p7 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), 1, 1);
		vec4 p8 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), 1, 1);

		// Two points are used by two triangles each
		vec4 newpoints[36] = {	
								p1, p3, p2, //Front
								p2, p3, p4,

								p1, p2, p5, //left
								p5, p2, p6, 
								
								p5, p6, p7, //back
								p7, p6, p8,

								p7, p4, p3, //right
								p4, p7, p8,

								p2, p4, p6, //top
								p6, p4, p8, 

								p1, p3, p5, //bottom
								p5, p3, p7
							}; 

		
		glBufferSubData(GL_ARRAY_BUFFER, i*36*sizeof(vec4), 36*sizeof(vec4), newpoints); 
	}

	if(!isLegalPos(getTilePosition()) && !isDropped) { //turn tile grey if the position is illegal and the tile has not been dropped
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(greycolours), greycolours); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else if(!isDropped){
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newColors), newColors); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}


// Called at the start of the game and when the tile is set
void newTile(){
	tilepos = vec2(5 , 19); // Put the tile at the top of the board
	int currentColor = 0;
	int tileType = random(5);
	currentTileOrientation = DOWN;
	currentTileType = tileType;
	// Update the geometry VBO of current tile
	for (int i = 0; i < 6; i++)
		switch(tileType) {
			case 0:
				tile[i] = allRotationsLshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case 1:
				tile[i] = allRotationsTshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case 2:
				tile[i] = allRotationsSshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case 3:
				tile[i] = allRotationsIshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case 4:
				tile[i] = allRotationsMirroredLshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case 5:
				tile[i] = allRotationsZshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;/**/
			default:
				break;
		}
	tileUpdate(); 

	// Update the color VBO of current tile
	for (int i = 0; i < 144; i++){
		if((i%36)==0){
			currentColor = random(4);
		}

		newColors[i] = colors[currentColor]; 
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]); 
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newColors), newColors); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	if(board[5][19] == true || board[4][19] == true || board[6][19] == true) {
		printf("Cannot spawn new tile! Game exiting in 1 second...\n");
		sleep(1);
		exit(EXIT_SUCCESS);
	}
}


void initGrid(){
	// Create grid points
	vec4 gridpoints[128]; // 32 lines --> 64 points 
	vec4 gridcolours[128]; // One colour per vertex
	// Vertical lines 
	for (int i = 0; i < 11; i++){
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 33.0, 1);
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 33.0, 1);
		
		gridpoints[22+2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[22+2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);
		
	}
	// Horizontal lines
	for (int i = 0; i < 21; i++){
		gridpoints[44 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 33.0, 1);
		gridpoints[44 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 33.0, 1);

		gridpoints[86+2*i] = vec4(35.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[86+2*i + 1] = vec4(365.0, (33.0 + (33.0 * i)), 0, 1);
	}
	// Make all grid lines white
	for (int i = 0; i < 128; i++)
		gridcolours[i] = white;


	// Setting up buffers --> Used learnopengl.com for guidance
	// Set up first VAO (representing grid lines)
	glBindVertexArray(VAOs[0]); 
	glGenBuffers(2, VBOs); 

	// Grid vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, 128*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(vPosition); 
	
	// Grid vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); 
	glBufferData(GL_ARRAY_BUFFER, 128*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor); 
}


void initBoard(){
	
	vec4 boardpoints[1200]; //FRONT - ORIGINAL
	vec4 boardpoints_rev[1200]; //BACK - IDENTICAL TO FRONT, COLOR DIRECTION REVERSED
	vec4 boardpoints_right[1200]; //RIGHT SIDE - STARTING FROM 1-10
	vec4 boardpoints_left[1200]; //LEFT SIDE - STARTING FROM 0-9
	vec4 boardpoints_bottom[1200]; //UNDER-SIDE - STARTING FROM 0-19
	vec4 boardpoints_top[1200]; //TOP-SIDE - STARTING FROM 1-20


	for (int i = 0; i < 1200; i++) boardColors[i] = black; // Empty cells on the board are black
	// Each cell is a square (2 triangles with 6 vertices)

	GLfloat z_pos1, z_pos2;
	
	z_pos1 = 1.0;
	z_pos2 = 32.0;

	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 10; j++)
		{	

			vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos1, 1);
			vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos1, 1);
			vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos1, 1);
			vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos1, 1);
			// Two points are reused
			boardpoints[6*(10*i + j)    ] = p1;
			boardpoints[6*(10*i + j) + 1] = p2;
			boardpoints[6*(10*i + j) + 2] = p3;
			boardpoints[6*(10*i + j) + 3] = p2;
			boardpoints[6*(10*i + j) + 4] = p3;
			boardpoints[6*(10*i + j) + 5] = p4;

			p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos2, 1);
			p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos2, 1);
			p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos2, 1);
			p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos2, 1);		
			// Set to reverse side
			boardpoints_rev[6*(10*i + j)    ] = p1;
			boardpoints_rev[6*(10*i + j) + 1] = p2;
			boardpoints_rev[6*(10*i + j) + 2] = p3;
			boardpoints_rev[6*(10*i + j) + 3] = p3;
			boardpoints_rev[6*(10*i + j) + 4] = p4;
			boardpoints_rev[6*(10*i + j) + 5] = p2;

			p1 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos1, 1);
			p2 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos1, 1);
			p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos2, 1);
			p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos2, 1);		
			// right side
			boardpoints_right[6*(10*i + j)    ] = p1;
			boardpoints_right[6*(10*i + j) + 1] = p3;
			boardpoints_right[6*(10*i + j) + 2] = p2;
			boardpoints_right[6*(10*i + j) + 3] = p2;
			boardpoints_right[6*(10*i + j) + 4] = p3;
			boardpoints_right[6*(10*i + j) + 5] = p4;

			p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos1, 1);
			p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos1, 1);
			p3 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos2, 1);
			p4 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos2, 1);		
			// left side
			boardpoints_left[6*(10*i + j)    ] = p1;
			boardpoints_left[6*(10*i + j) + 1] = p3;
			boardpoints_left[6*(10*i + j) + 2] = p2;
			boardpoints_left[6*(10*i + j) + 3] = p2;
			boardpoints_left[6*(10*i + j) + 4] = p4;
			boardpoints_left[6*(10*i + j) + 5] = p3;

			p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos1, 1);
			p2 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos2, 1);
			p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos1, 1);
			p4 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), z_pos2, 1);		
			// bottom side
			boardpoints_bottom[6*(10*i + j)    ] = p1;
			boardpoints_bottom[6*(10*i + j) + 1] = p2;
			boardpoints_bottom[6*(10*i + j) + 2] = p3;
			boardpoints_bottom[6*(10*i + j) + 3] = p2;
			boardpoints_bottom[6*(10*i + j) + 4] = p3;
			boardpoints_bottom[6*(10*i + j) + 5] = p4;

			p1 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos1, 1);
			p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos2, 1);
			p3 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos1, 1);
			p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), z_pos2, 1);		
			// top side
			boardpoints_top[6*(10*i + j)    ] = p1;
			boardpoints_top[6*(10*i + j) + 1] = p3;
			boardpoints_top[6*(10*i + j) + 2] = p2;
			boardpoints_top[6*(10*i + j) + 3] = p2;
			boardpoints_top[6*(10*i + j) + 4] = p3;
			boardpoints_top[6*(10*i + j) + 5] = p4;

		}
	}

	// Initially no cell is occupied
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++){
			board[i][j] = false;
			boardCellColors[i][j] = -1; 
		}

	// BACK 
	// set up buffer objects
	glBindVertexArray(VAOs[1]);
	glGenBuffers(2, &VBOs[2]);

	// Grid cell vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Grid cell vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// FRONT
	glBindVertexArray(VAOs[3]);
	glGenBuffers(2, &VBOs[6]);

	// Grid cell vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[6]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_rev, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Grid cell vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// RIGHT
	glBindVertexArray(VAOs[4]);
	glGenBuffers(2, &VBOs[8]);

	// Grid cell vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[8]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_right, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Grid cell vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// LEFT
	glBindVertexArray(VAOs[5]);
	glGenBuffers(2, &VBOs[10]);

	// Grid cell vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[10]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_left, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Grid cell vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// BOTTOM
	glBindVertexArray(VAOs[6]);
	glGenBuffers(2, &VBOs[12]);

	// Grid cell vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[12]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_bottom, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Grid cell vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// TOP
	glBindVertexArray(VAOs[7]);
	glGenBuffers(2, &VBOs[14]);

	// Grid cell vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[14]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints_top, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Grid cell vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

}

// No geometry for current tile initially
void initCurrentTile(){
	glBindVertexArray(VAOs[2]);
	glGenBuffers(2, &VBOs[4]);

	// Current tile vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, 4*36*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Current tile vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);
	glBufferData(GL_ARRAY_BUFFER, 4*36*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
}

void initCube(){
	cube();

	glBindVertexArray(VAOs[8]);
	glGenBuffers(2, &VBOs[16]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[16]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(c_points), c_points, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[17]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(c_colors), c_colors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
}

void init(){
	// Load shaders and use the shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl"); //slightly modified shaders from provided Angel files
	glUseProgram(program);

	vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(9, &VAOs[0]);

	for(int i = 0; i<144; i++){
		greycolours[i] = darkgrey;
	}

	initCurrentTile();
	initGrid();
	initBoard();
	initCube();

	isDropped = false;
	newTile(); // create new next tile

	// set to default
	ModelView = glGetUniformLocation( program, "ModelView" );
	glEnable( GL_DEPTH_TEST );

	cameraView = LookAt(vec3(0.0, 30.0, 30.0),	vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0));

	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);
}

bool checkRotation(){
	int xPos = tilepos.x;
	int yPos = tilepos.y;

	vec2 allCellPos[4] = shapes[currentTileType][currentTileOrientation]; //array of cell positions for this tile
	for(int j = 0; j<4; ++j){ //Load positions for all tile cells into an array 
		allCellPos[j].x += xPos;
		allCellPos[j].y += yPos;
	}

	for(int i = 0; i<4; ++i){
		if(board[(int)allCellPos[i].x][(int)allCellPos[i].y]==true) return false;
	}

	bool move_attempt;

	switch(currentTileType) {
		case L:	
			if(xPos == 0) {
				move_attempt = moveTile(vec2(1,0));
				return move_attempt;
			}
			else if(xPos == 9) {
				move_attempt = moveTile(vec2(-1,0));
				return move_attempt;
			}
			else if(yPos == 0) { //no floor kick rotations
				return false;
			}
			else if(yPos==19) {
				move_attempt = moveTile(vec2(0,-1));
				return move_attempt;
			}	
			break;
		case T:
			if(xPos == 0) {
				move_attempt = moveTile(vec2(1,0));
				return move_attempt;
			}
			else if(xPos == 9) {
				move_attempt = moveTile(vec2(-1,0));
				return move_attempt;
			}
			else if(yPos == 0) { //no floor kick rotations
				return false;
			}
			else if(yPos==19) {
				move_attempt = moveTile(vec2(0,-1));
				return move_attempt;
			}
			break;
		case S:
			if(xPos == 0) {
				move_attempt = moveTile(vec2(1,0));
				return move_attempt;
			}
			else if(yPos==19) {
				move_attempt = moveTile(vec2(0,-1));
				return move_attempt;
			}
			break;
		case I:
			if(xPos == 0) {
				move_attempt = moveTile(vec2(2,0));
				return move_attempt;
			}
			else if(xPos == 1){
				move_attempt = moveTile(vec2(1,0));
				return move_attempt;
			}
			else if(xPos == 9) {
				move_attempt = moveTile(vec2(-1,0));
				return move_attempt;
			}
			else if(yPos == 0 || yPos == 1) { //no floor kick rotations
				return false;
			}
			else if(yPos==19) {
				move_attempt = moveTile(vec2(0,-1));
				return move_attempt;
			}
			break;
		case ML:	
			if(xPos == 0) {
				move_attempt = moveTile(vec2(1,0));
				return move_attempt;
			}
			else if(xPos == 9) {
				move_attempt = moveTile(vec2(-1,0));
				return move_attempt;
			}
			else if(yPos == 0) { //no floor kick rotations
				return false;
			}
			else if(yPos==19) {
				move_attempt = moveTile(vec2(0,-1));
				return move_attempt;
			}	
			break;
		case Z:
			if(xPos == 0) {
				move_attempt = moveTile(vec2(1,0));
				return move_attempt;
			}
			else if(yPos==19) {
				move_attempt = moveTile(vec2(0,-1));
				return move_attempt;
			}
			break;
		default:
			break;
	}
	return true;
}
// Rotates the current tile, if there is room
void changeOrientation(){
	for (int i = 0; i < 6; i++)
		switch(currentTileType) {
			case L:
				tile[i] = allRotationsLshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case T:
				tile[i] = allRotationsTshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case S:
				tile[i] = allRotationsSshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case I:
				tile[i] = allRotationsIshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case ML:
				tile[i] = allRotationsMirroredLshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			case Z:
				tile[i] = allRotationsZshape[currentTileOrientation][i]; // Get the 4 pieces of the new tile
				break;
			default:
			break;
		}
}

void rotate(){      
	switch(currentTileOrientation){
		case DOWN:
			currentTileOrientation = RIGHT;
			changeOrientation();
			if(!checkRotation()){
				currentTileOrientation = DOWN;
				changeOrientation();
			}
			break;
		case RIGHT:
			currentTileOrientation = UP;
			changeOrientation();
			if(!checkRotation()){
				currentTileOrientation = RIGHT;
				changeOrientation();
			}
			break;
		case UP:
			currentTileOrientation = LEFT;
			changeOrientation();
			if(!checkRotation()){
				currentTileOrientation = UP;
				changeOrientation();
			}
			break;
		case LEFT:
			currentTileOrientation = DOWN;
			changeOrientation();
			if(!checkRotation()){
				currentTileOrientation = LEFT;
				changeOrientation();
			}
			break;
		default:
			break;
	}
	tileUpdate();
}


bool checkForColorMatches(){
	int colLower, colHigher, rowLower, rowHigher, candidate; //ranges for potential cell deletion

	colLower = 0; colHigher = 0; //initialized to first column entry
	candidate = boardCellColors[0][0]; 

	int intsize = sizeof(boardCellColors[0][0]);

	for(int i = 0; i<10; ++i){ //check for column matches
		colLower = 0; colHigher = 0;
		candidate = boardCellColors[i][0];

		for(int j = 0; j<20; ++j){
			if(boardCellColors[i][j]==-1 && !(colHigher-colLower>2)){
				candidate = -1; colLower = colHigher; colLower++; colHigher++;
			}
			else if(boardCellColors[i][j]!=candidate && !(colHigher-colLower>2)){
				candidate = boardCellColors[i][j]; colLower = colHigher; colHigher++;
			}
			else if(boardCellColors[i][j]!=candidate && (colHigher-colLower>2)){				
				
				memmove(board[i]+colLower, board[i]+colHigher, 20-1-colHigher);
				memmove(boardCellColors[i]+colLower, boardCellColors[i]+colHigher, 20*intsize-intsize-intsize*colHigher); //possibility of integer size difference between systems
				board[i][19]=false;
				boardCellColors[i][19]=-1;

				for(int k = 0; k<(19-colHigher); ++k){					
					memmove(boardColors+((k+colLower)*60)+(i*6), boardColors+((k+colHigher)*60)+(i*6), 96);
				}
				boardColors[19*60+(i*6)] = black;
				checkForColorMatches();
				return true;
				
				candidate = boardCellColors[i][j]; colHigher=colLower; colHigher++;
			}
			else if(boardCellColors[i][j]==candidate){
				colHigher++;
			}
		}
	}

	for(int j = 0; j<20; ++j){ //now check for row-matches
		rowLower = 0; rowHigher = 0;
		candidate = boardCellColors[0][j];

		for(int i = 0; i<10; ++i){
			if(boardCellColors[i][j]==-1 && !(rowHigher-rowLower>2)){
				candidate = -1; rowLower = rowHigher; rowLower++; rowHigher++;
			}
			else if(boardCellColors[i][j]!=candidate && !(rowHigher-rowLower>2)){
				candidate = boardCellColors[i][j]; rowLower = rowHigher; rowHigher++;
			}
			else if(boardCellColors[i][j]!=candidate && (rowHigher-rowLower>2)){ 
				for(int h = 0; h<(rowHigher-rowLower); ++h){
					memmove(board[rowLower+h]+j, board[rowLower+h]+j+1, 20-j-1);
					memmove(boardCellColors[rowLower+h]+j, boardCellColors[rowLower+h]+j+1, 20*intsize-intsize-intsize*j); //possibility of integer size difference between systems
					board[rowLower+h][19]=false;
					boardCellColors[rowLower+h][19]=-1;
				}

				for(int k = 0; k<(19-j); ++k){					
					memmove(boardColors+((k+j)*60)+(rowLower*6), boardColors+((k+1+j)*60)+(rowLower*6), 96*(rowHigher-rowLower));
				}
				for(int h = rowLower; h<rowHigher; ++h){
					boardColors[19*60+(h*6)] = black;
				}
				checkForColorMatches();

				return true;

			}
			else if(boardCellColors[i][j]==candidate){
				rowHigher++;
			}	
		}

	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);

	return true;	
}

// Checks if the specified row (0 is the bottom 19 the top) is full
// If every cell in the row is occupied, it will clear that cell and everything above it will shift down one row
void checkFullRow(int row){
	for(int k = 0; k<10; ++k){
		if(board[k][row]==false) {
			return;
		}
	}

	int intsize = sizeof(boardCellColors[0][0]);

	for(int i = 0; i<10; ++i){ //move the cell detection table and the color lookup table rows column by column
		memmove(board[i]+row, board[i]+row+1, 20-1-row);
		memmove(boardCellColors[i]+row, boardCellColors[i]+row+1, 20*intsize-intsize-intsize*row); //possibility of integer size difference between systems
		board[i][19]=false;
		boardCellColors[i][19]=-1;
	}
	
	memmove(boardColors+(row*60), boardColors+((row+1)*60), (18240)-((row)*60*16));
	for(int j = 1140; j< 1200; ++j){
		boardColors[j] = black;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);

	checkForColorMatches();
}

// Places the current tile - update the board vertex colour VBO and the array maintaining occupied cells
void setTile(){

	int low = 20; int high = -1;
	int xPos = tilepos.x;
	int yPos = tilepos.y;

	vec2 allCellPos[4] = shapes[currentTileType][currentTileOrientation]; //array of cell positions for coloring

	for(int j = 0; j<4; ++j){
		allCellPos[j].x += xPos;
		allCellPos[j].y += yPos;
		if((int)allCellPos[j].y > high) high = (int)allCellPos[j].y;
		if((int)allCellPos[j].y < low) low = (int)allCellPos[j].y;
	}


	for(int i = 0; i < 4; ++i){ //for each cell in the current tile
		for(int k = 0; k < 6; ++k){ //for each vertex in the current cell
			boardColors[(60*(int)allCellPos[i].y + 6*(int)allCellPos[i].x) + k] = {newColors[k+(i*36)][0], newColors[k+(i*36)][1], newColors[k+(i*36)][2], newColors[k+(i*36)][3]};
		}

		if((newColors[i*36][0] == 0.0f) && (newColors[i*36][1] == 1.0f) && (newColors[i*36][2] == 0.0f) && (newColors[i*36][3] == 1.0f)){ //GREEN
			boardCellColors [(int)allCellPos[i].x][(int)allCellPos[i].y] = GREEN;
		}
		else if((newColors[i*36][0] == 0.0f) && (newColors[i*36][1] == 0.0f) && (newColors[i*36][2] == 1.0f) && (newColors[i*36][3] == 1.0f)){ //BLUE
			boardCellColors [(int)allCellPos[i].x][(int)allCellPos[i].y] = BLUE;
		}
		else if((newColors[i*36][0] == 1.0f) && (newColors[i*36][1] == 0.0f) && (newColors[i*36][2] == 0.0f) && (newColors[i*36][3] == 1.0f)){ //RED
			boardCellColors [(int)allCellPos[i].x][(int)allCellPos[i].y] = RED;
		}
		else if((newColors[i*36][0] == 1.0f) && (newColors[i*36][1] == 0.5f) && (newColors[i*36][2] == 0.0f) && (newColors[i*36][3] == 1.0f)){ //ORANGE
			boardCellColors [(int)allCellPos[i].x][(int)allCellPos[i].y] = ORANGE;
		}
		else if((newColors[i*36][0] == 1.0f) && (newColors[i*36][1] == 1.0f) && (newColors[i*36][2] == 0.0f) && (newColors[i*36][3] == 1.0f)){ //YELLOW
			boardCellColors [(int)allCellPos[i].x][(int)allCellPos[i].y] = YELLOW;
		}



		board[(int)allCellPos[i].x][(int)allCellPos[i].y] = true;

	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardColors, GL_DYNAMIC_DRAW);

	//At this point, call checkrow
	//printf("Low: %d, high: %d\n", low, high);
	for(int l = high; l >=low; --l){ //check from the top down so that memmove operation won't affect where rows are
		checkFullRow(l);
	}

	//now check for consecutive matching colors in columns/rows
	checkForColorMatches();

	newTile();
	isDropped = false;
}

bool moveTile(vec2 direction){
	int leftBorder, rightBorder, botBorder;
	leftBorder = 0; rightBorder = 0; botBorder = 0;

	switch(currentTileType) {
		case L: //0
			switch(currentTileOrientation) {
				case DOWN:
					leftBorder = 1; rightBorder = 8; botBorder = 1;
					break;
				case RIGHT:
					leftBorder = 0; rightBorder = 8; botBorder = 1;
					break;
				case UP:
					leftBorder = 1; rightBorder = 8; botBorder = 0;
					break;
				case LEFT:
					leftBorder = 1; rightBorder = 9; botBorder = 1;
					break;
				default:
					break;
			}
			break;
		case T: //1
			switch(currentTileOrientation) {
				case DOWN:
					leftBorder = 1; rightBorder = 8; botBorder = 1;
					break;
				case RIGHT:
					leftBorder = 0; rightBorder = 8; botBorder = 1;
					break;
				case UP:
					leftBorder = 1; rightBorder = 8; botBorder = 0;
					break;
				case LEFT:
					leftBorder = 1; rightBorder = 9; botBorder = 1;
					break;
				default:
					break;
			}
			break;
		case S: //2
			switch(currentTileOrientation) {
				case DOWN:
					leftBorder = 1; rightBorder = 8; botBorder = 1;
					break;
				case RIGHT:
					leftBorder = 0; rightBorder = 8; botBorder = 1;
					break;
				case UP:
					leftBorder = 1; rightBorder = 8; botBorder = 1;
					break;
				case LEFT:
					leftBorder = 0; rightBorder = 8; botBorder = 1;
					break;
				default:
					break;
			}
			break;
		case I: //3
			switch(currentTileOrientation) {
				case DOWN:
					leftBorder = 2; rightBorder = 8; botBorder = 0;
					break;
				case RIGHT:
					leftBorder = 0; rightBorder = 9; botBorder = 2;
					break;
				case UP:
					leftBorder = 2; rightBorder = 8; botBorder = 0;
					break;
				case LEFT:
					leftBorder = 0; rightBorder = 9; botBorder = 2;
					break;
				default:
					break;
			}
			break;
		case ML: //4
			switch(currentTileOrientation) {
				case DOWN:
					leftBorder = 1; rightBorder = 8; botBorder = 1;
					break;
				case RIGHT:
					leftBorder = 0; rightBorder = 8; botBorder = 1;
					break;
				case UP:
					leftBorder = 1; rightBorder = 8; botBorder = 0;
					break;
				case LEFT:
					leftBorder = 1; rightBorder = 9; botBorder = 1;
					break;
				default:
					break;
			}
			break;
		case Z: //5
			switch(currentTileOrientation) {
				case DOWN:
					leftBorder = 1; rightBorder = 8; botBorder = 1;
					break;
				case RIGHT:
					leftBorder = 0; rightBorder = 8; botBorder = 1;
					break;
				case UP:
					leftBorder = 1; rightBorder = 8; botBorder = 1;
					break;
				case LEFT:
					leftBorder = 0; rightBorder = 8; botBorder = 1;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	int xPos = tilepos.x;
	int yPos = tilepos.y;

	vec2 allCellPos[4] = shapes[currentTileType][currentTileOrientation]; //array of cell positions for this tile
	for(int j = 0; j<4; ++j){ //Load positions for all tile cells into an array 
		allCellPos[j].x += xPos;
		allCellPos[j].y += yPos;
	}

	for(int k = 0; k<4; ++k){
		if((board[(int)allCellPos[k].x][(int)allCellPos[k].y-1]==true) && (direction.y!=0)){
			setTile();
			return false;
		} 
	}

	if(tilepos.y==botBorder && direction.y!=0) {
		setTile();
		return false;
	}

	if(tilepos.y+direction.y >= botBorder) tilepos.y += direction.y;
	if((tilepos.x+direction.x >= leftBorder) && 
		(tilepos.x+direction.x <= rightBorder)){ 
		for(int k = 0; k<4; ++k){
			if(board[(int)allCellPos[k].x + (int)direction.x][(int)allCellPos[k].y]==true){
				return false;
			}
		}
		tilepos.x += direction.x;
	}
	return true;
}

// Reset the board, tiles and current piece
void restart()
{
	init();
}

//Robot arm functions

void base(){
	mystack.push(modelView);
	glBindVertexArray(VAOs[8]);
	mat4 instance = (Translate(-33.0, 0.0, 33.0)*Scale(99.0, 33.0, 99.0));
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	modelView = mystack.pop();
}

void lower(){
	mystack.push(modelView);
	glBindVertexArray(VAOs[8]);
	mat4 instance = (
		Translate(0.0, 0.0, 38.0)
		*Scale(ROBOT_LOWER_ARM_WIDTH, ROBOT_LOWER_ARM_HEIGHT, ROBOT_LOWER_ARM_WIDTH)
		);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	modelView = mystack.pop();
}

void upper(){
	mystack.push(modelView);
	glBindVertexArray(VAOs[8]);
	mat4 instance = (
		Translate(10.0, 0.0, 38.0)
		*Scale(ROBOT_UPPER_ARM_WIDTH, ROBOT_UPPER_ARM_HEIGHT, ROBOT_UPPER_ARM_WIDTH)
		);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	modelView = mystack.pop();
}

void tiledropper(){
	mystack.push(modelView);
	glBindVertexArray(VAOs[2]);
	mat4 instance = (
		Scale(1.0, 1.0, 1.0) *
		Translate(-198.0, -660.0, 33.0)
		);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView * instance);
	glDrawArrays(GL_TRIANGLES, 0, 4*36);
	modelView = mystack.pop();
}

void boardGridTileDraw(){
	glBindVertexArray(VAOs[1]); // Bind the VAO representing the front of the board 
	glDrawArrays(GL_TRIANGLES, 0, 1200); //Draw the board front 

	if(isDropped){
		glBindVertexArray(VAOs[2]); // Bind the VAO representing the current tile
		glDrawArrays(GL_TRIANGLES, 0, 4*36); // Draw the current tile
	}

	glBindVertexArray(VAOs[3]); // Bind the VAO representing the back of the board
	glDrawArrays(GL_TRIANGLES, 0, 1200); // Draw the board back

	glBindVertexArray(VAOs[4]); // Bind the VAO representing left of the board
	glDrawArrays(GL_TRIANGLES, 0, 1200); // Draw the board left

	glBindVertexArray(VAOs[5]); // Bind the VAO representing right of the board
	glDrawArrays(GL_TRIANGLES, 0, 1200); // Draw the board right

	glBindVertexArray(VAOs[6]); // Bind the VAO representing the board bottom
	glDrawArrays(GL_TRIANGLES, 0, 1200); // Draw the board bottom

	glBindVertexArray(VAOs[7]); // Bind the VAO representing top of the board
	glDrawArrays(GL_TRIANGLES, 0, 1200); // Draw the board top	

	glBindVertexArray(VAOs[0]); // Bind the VAO representing the grid lines (to be drawn on top of everything else)
	glDrawArrays(GL_LINES, 0, 128); // Draw the grid lines
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projectionView = Perspective(45.0, 1.0*xsize/ysize, 5.0, 300.0); //setting viewing space

	mat4 M = mat4();
	M *= Translate(0.0, 10.0, 0);
	M *= Scale(1.0/33.0, 1.0/33.0, 1.0/33.0);
	M *= Translate(-200.0, -360.0, 0.0);

	modelView = projectionView * cameraView * M;

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelView);

	boardGridTileDraw();
	
	base();

	mystack.push(modelView);
	modelView *= ( 
		Translate(0.0, ROBOT_BASE_HEIGHT, 0.0) *
		RotateZ(phiAngle[LowerArm])
		);

	lower();

	modelView *= (
		Translate(0.0, ROBOT_LOWER_ARM_HEIGHT, 0.0) *
		RotateZ(phiAngle[UpperArm])
		);

	upper();
	
	modelView *= (
		Translate(0.0, ROBOT_UPPER_ARM_HEIGHT, 0.0) * 
		RotateZ(-phiAngle[UpperArm]) *
		RotateZ(-phiAngle[LowerArm])
		);

	if(!isDropped) tiledropper();
	
	modelView = mystack.pop();

	glutSwapBuffers();
}


// Reshape callback will simply change xsize and ysize variables, which are passed to the vertex shader
// to keep the game the same from stretching if the window is stretched
void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	zsize = w;	
	glViewport(0, 0, w, h);
}
/*
void shuffleColors(){		//dead tile func
	vec4 temp[6];
	memcpy(temp, newColors, 96);
	memmove(newColors, newColors+6, 288);
	memcpy(newColors+18, temp, 96);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]); // Bind the VBO containing current tile vertex colours
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newColors), newColors); // Put the colour data in the VBO

}
*/
void dropTile(){ //assume tile is in a legal position	
	vec2 curTilePos = getTilePosition();
	int xPos = curTilePos.x;
	int yPos = curTilePos.y;

	tilepos.x = xPos-1;
	tilepos.y = yPos-1;

	tileUpdate();
	isDropped = true;
	
}

bool isLegalPos(vec2 curTilePos){	
	bool result = true;
	int xPos = curTilePos.x;
	int yPos = curTilePos.y;

	vec2 allCellPos[4] = shapes[currentTileType][currentTileOrientation]; //array of cell positions for this tile
	for(int j = 0; j<4; ++j){ //Load positions for all tile cells into an array 
		allCellPos[j].x += xPos;
		allCellPos[j].y += yPos;
	}

	/*Make sure they are inside the board, and not in an occupied space*/

	for(int i = 0; i<4; i++){
		if( allCellPos[i].x < 1 
			|| allCellPos[i].x > 10 
			|| allCellPos[i].y < 1 
			|| allCellPos[i].y > 20) result = false;

		if(board[(int)allCellPos[i].x-1][(int)allCellPos[i].y-1] == true) result = false;
	}

	return result;	
}

void special_up(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			rate = 0.0f;
			break;
		case GLUT_KEY_RIGHT:
			rate = 0.0f;
			break;
	}
}

//Arrow key presses
void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			if(isDropped){
				rotate();
				tileUpdate();
			}
			break;
		case GLUT_KEY_DOWN:
			if(isDropped){
				moveTile(vec2(0,-1));
				tileUpdate();
			}
			break;
		case GLUT_KEY_LEFT:
			if(glutGetModifiers() & GLUT_ACTIVE_CTRL) { //ctrl+left
				Axis = Yaxis;
				rate = 2.0f;
				cameraView *= RotateY(rate);
				break;
			}
			break;
		case GLUT_KEY_RIGHT:
			if(glutGetModifiers() & GLUT_ACTIVE_CTRL) {
				Axis = Yaxis;
				rate = -2.0f;
				cameraView *= RotateY(rate);
				break;
			}
			break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'w':
			angle = UpperArm;
			if(phiAngle[angle] < 0) phiAngle[angle] += 1;			
			if(!isDropped) tileUpdate();
			break;
		case 's':
			angle = UpperArm;
			if(phiAngle[angle] > -180) phiAngle[angle] -= 1;
			if(!isDropped) tileUpdate();
			break;
		case 'a':
			angle = LowerArm;
			if(phiAngle[angle] < 90) phiAngle[angle] += 1;
			if(!isDropped) tileUpdate();
			break;
		case 'd':
			angle = LowerArm;
			if(phiAngle[angle] > -90) phiAngle[angle] -= 1;
			if(!isDropped) tileUpdate();
			break;
		case ' ':
			if(!isDropped && isLegalPos(getTilePosition())){
				dropTile();
			} 
			break;
		case 033: // Both escape key and 'q' cause the game to exit
		    exit(EXIT_SUCCESS);
		    break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'r': // 'r' key restarts the game
			restart();
			break;
	}	
	glutPostRedisplay();
}

void idle(void)
{
	glutPostRedisplay();
}

void timedMove(int t){ 
	if(isDropped){
		moveTile(vec2(0, -1));
		tileUpdate();
	}
	glutTimerFunc(1000, timedMove, 0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 178); 
	glutCreateWindow("FruitTetris3D");
	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutSpecialUpFunc(special_up);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutTimerFunc(1000, timedMove, 0);

	glutMainLoop();
	return 0;
}

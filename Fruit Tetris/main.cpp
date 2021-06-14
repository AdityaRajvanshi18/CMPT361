//main.cpp
#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include "Board.cpp"
#include "Game.cpp"
#include "Block.cpp"

#define TILEFALLSPEED 150

using namespace std;

Board board;
Game game;
Block current_moving_block;

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 10.0, 0.0, 20.0);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	board.drawBoard();

	//if there is a complete row shift down
	game.shiftBlocksDown();

	if(game.isBlockTouchBlock(BOTTOM)){
		if(game.isBlockStoreFull()){
			//game over
			game.clearBlockStore();
			exit(EXIT_SUCCESS); 
			
		}
		else{
			//if block touches block save the current block
			//to the block store and start new shape
			
			current_moving_block.resetBlock();
			game.addBlockToBlockStore();
		}
	}
	else{
		current_moving_block.moveBottom(false);	
	}
	//apply changes of the block pos to the current
	//block
	game.updateSingleBlock(current_moving_block);
	//glColor3f(1.0f,1.0f,1.0f);
	game.drawBlockStore();

	glutSwapBuffers();
}

void timer_func(int n){
	glutPostRedisplay();
        glutTimerFunc(TILEFALLSPEED, timer_func, 0);
}

void keyboard_s (int key, int x, int y){
    switch (key){
        case GLUT_KEY_UP:
		current_moving_block.rotateShape();
        break;
        case GLUT_KEY_DOWN:	
		//move the block until it touches other block
		while( ! game.isBlockTouchBlock(BOTTOM)){
			current_moving_block.moveBottom(true);
			game.updateSingleBlock(current_moving_block);
		}
        break;
        case GLUT_KEY_LEFT:
		if(! game.isBlockTouchBlock(LEFT)){
			current_moving_block.moveLeft();
			game.updateSingleBlock(current_moving_block);
		}
			
        break;
        case GLUT_KEY_RIGHT:
		if(! game.isBlockTouchBlock(RIGHT)){
			current_moving_block.moveRight();
			game.updateSingleBlock(current_moving_block);
		}
        break;
	}
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 033: //escape and q key casue the game to exit
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit(EXIT_SUCCESS);
			break;
		case 'r':
			game.clearBlockStore();
			current_moving_block.resetBlock();
	}		
}
/*void idle(){
	glutPostRedisplay();
}
*/
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(800, 50);
	glutInitWindowSize(400, 800);
	glutCreateWindow("Fruit Tetris");

	init();
	glutTimerFunc(TILEFALLSPEED, timer_func, 0);
	glutDisplayFunc(display);
	//glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc (keyboard_s);
	
	glutMainLoop();
}



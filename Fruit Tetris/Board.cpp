/*
	Board.cpp
	Aditya Rajvanshi
*/

#include <GL/glut.h>
#include <iostream>
#include "Board.h"

using namespace std;

//Constructor
Board::Board(){
	//board[10][20];
	for (int j = 0; j < 21; j++){
		for (int i = 0; i < 11; i++){
			cellOccupied[i][j] = false;
			cellColour[i][j] = 1;
		}
	}
}	

void Board::drawBoard(){	
glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	for (int j = 0; j < 21; j++){
		for (int i = 0; i < 11; i++){

			

			glColor3f(1.0, 1.0, 1.0);
			//Vertical Lines
			glBegin(GL_LINES);
				glVertex2i(i, j);
				glVertex2i(i, (j + disp));
			glEnd();
			//Horizontal lines
			glBegin(GL_LINES);
				glVertex2i(i, j);
				glVertex2i((i + disp), j);
			glEnd();
			cellOccupied[i][j] = false;

			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_POLYGON);
				glVertex2i(i,j);
				glVertex2i((i + disp), j);
				glVertex2i((i + disp), (j + disp));
				glVertex2i(i,(j + disp));
			glEnd();
		}
	}
	
	glFlush();
	glutSwapBuffers();
}

//get whether cell[x][y] is occupied
bool Board::getCellOccupied(int x, int y){ 
	return cellOccupied[x][y];
}

//return true if cell[x][y] occupation set successfully
bool Board::setCellOccupied(int x, int y){ 
	cellOccupied[x][y] = true;
}

//Set Board Colour
//Use by placing tile at board[x][y] then call setCellColour(x, y, colourCode)
//Give placeTile colourCode and use if statements to set colour at that point in the board
void Board::setCellColour(int x, int y, int colourCode){

	if(colourCode = 1){
		cellColour[x][y]= colourCode;
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2i(x,y);
			glVertex2i((x+disp),y);
			glVertex2i((x + disp), (y +disp));
			glVertex2i(x,(y +disp));
		glEnd();
	}
	else if(colourCode = 2){
		cellColour[x][y]= colourCode;
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2i(x,y);
			glVertex2i((x+disp),y);
			glVertex2i((x + disp), (y +disp));
			glVertex2i(x,(y +disp));
		glEnd();
	}	

	else if(colourCode = 3){
		cellColour[x][y]= colourCode;
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
			glVertex2i(x,y);
			glVertex2i((x+disp),y);
			glVertex2i((x + disp), (y +disp));
			glVertex2i(x,(y +disp));
		glEnd();
	}

	else if(colourCode = 4){
		cellColour[x][y]= colourCode;
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2i(x,y);
			glVertex2i((x+disp),y);
			glVertex2i((x + disp), (y +disp));
			glVertex2i(x,(y +disp));
		glEnd();
	}

	else if(colourCode = 5){
		cellColour[x][y]= colourCode;
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2i(x,y);
			glVertex2i((x+disp),y);
			glVertex2i((x + disp), (y +disp));
			glVertex2i(x,(y +disp));
		glEnd();
	}


	else if(colourCode = 6){
		cellColour[x][y]= 1;
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
			glVertex2i(x,y);
			glVertex2i((x+disp),y);
			glVertex2i((x + disp), (y + disp));
			glVertex2i(x,(y + disp));
		glEnd();
	}

	else if(colourCode = 7){
		cellColour[x][y]= colourCode;
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
			glVertex2i(x,y);
			glVertex2i((x+disp),y);
			glVertex2i((x + disp), (y +disp));
			glVertex2i(x,(y +disp));
		glEnd();
	}
	else{
		cout << "CELL COLOUR NOT CHANGED" << endl;
	}

	glFlush();
	glutSwapBuffers();


}






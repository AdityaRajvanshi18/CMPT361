/*
	Game.cpp
	Aditya Rajvanshi		
*/

#include <iostream>
#include "Game.h"
#include <GL/glut.h>
using namespace std;
int XVAL = 10;
int YVAL = 20;

Game::Game(){
	clearSingleBlock();
	clearBlockStore();
}
	
void Game::clearSingleBlock(){
	for(int j = 0; j<YVAL; j++){
		for(int i = 0; i<XVAL; i++){
			singleBlock[i][j] = 0;
		}
	}
}

void Game::clearBlockStore(){
	for(int j=0;j < YVAL;j++)
		{
			for(int i=0; i < XVAL;i++){
				blockStore[i][j]=0;
		}
	}
}

void Game::setSingleBlockElement(int col, int row, int value, int colourCode){
	if(row >= 0 && row < YVAL && col  >= 0 && col  <  XVAL){
		singleBlock[col][row] = value;
		singleBlockColour[col][row] = colourCode;
	}
}

int Game::getBlockStoreElement(int i, int j){
	if(j  >= 0 && j < YVAL && i  >= 0 && i < XVAL){
		//touch block
		return blockStore[i][j];
	}
	else{
		//touch the walls
		return -1;
	}
}

bool Game::isBlockTouchBlock(Sides side){
	if(side == BOTTOM){
		for(int j=0;j < YVAL;j++){
			for(int i=0; i < XVAL ;i++){
				if(singleBlock[i][j]  >  0){
					if(getBlockStoreElement(i,j-1)  >  0 || getBlockStoreElement(i,j-1) == -1){
						return true;
					}
				}
			}
		}
	}
	else if(side == RIGHT){
		for(int j=0; j < YVAL; j++){
				for(int i=0;i < XVAL;i++){
					if(singleBlock[i][j]  >  0){
						if(getBlockStoreElement(i+1,j)  < 0 || getBlockStoreElement(i+1,j) == 11){
							return true;
						}
					}
				}
			}
	}
	else if(side == LEFT){
		for(int j=0; j < YVAL; j++){
				for(int i=0;i < XVAL;i++){
					if(singleBlock[i][j]  >  0){
						if(getBlockStoreElement(i-1,j)  <  0 || getBlockStoreElement(i-1,j) == -1){
							return true;
						}
					}
				}
			}
	}
	return false;
}

bool Game::isBlockStoreFull(){
	for(int i=0;i < XVAL; i++){
			//if the top row has blocks
			if(blockStore[i][19]  >  0){
				return true;
			}
		}
		return false;
}
	
bool Game::isCompleteRow(int row){
	for(int i=0;i < XVAL; i++){
		//if you find an empty spot return false
		if(blockStore[i][row] == 0){ 
			return false;
		}
	}
	return true;
}

bool Game::isRowEmpty(int row){
	for(int i=0;i < XVAL; i++){
	//if any non zero value found, return false
		if(blockStore[i][row] != 0){ 
			return false;
		}
	}
	return true;
}

void Game::shiftBlocksDown(){
	for(int j = 0;j < YVAL;j++){
	
		if(isCompleteRow(j)){
			//do the shift starting from the row 
			//which is full moving backwords to 
			//reach the bottom of the board
			
			for(int i = 0; i <XVAL; i++){
					blockStore[i][j] = 0;
				for (int k = j+1; k < 19; k++){
					blockStore[i][k]=blockStore[i][k+1];
				}
			}
		}
	}
}



/*
void Game::shiftBlocksDown(){
	for(int j = 0;j < YVAL;j++){
	
		if(isCompleteRow(j)){
			bool temp = isCompleteRow(j);
			//do the shift starting from the row 
			//which is full moving backwords to 
			//reach the bottom of the board
 
			for(int k=j;k > -1;k--){
				//first clear the full row
				for(int i=0;i < XVAL; i++){
					blockStore[i][k]=0;
				}
				//second: move the data of the above row to
				//the row below
				for(int i=0;i < XVAL; i++){
					blockStore[i][k]=blockStore[i][k-1];
				}
			}
		}
	}
}
*/

void Game::drawBlockStore(){
	int combinedElement;	
	for(int j=0;j < YVAL ;j++){
		for(int i=0;i < XVAL;i++){
		combinedElement=blockStore[i][j]+singleBlock[i][j];		
			if(combinedElement!=0 ){ 				
				if(singleBlockColour[i][j] == 1){
					glColor3f(0.0, 1.0, 0.0);
					glBegin(GL_POLYGON);
						glVertex2i(i,j);
						glVertex2i((i + disp),j);
						glVertex2i((i + disp), (j +disp));
						glVertex2i(i,(j +disp));
					glEnd();
				}
				else if(singleBlockColour[i][j] ==  2){
					//cellColour[x][y]= colourCode;
					glColor3f(0.0, 1.0, 0.0);
					glBegin(GL_POLYGON);
						glVertex2i(i,j);
						glVertex2i((i+disp),j);
						glVertex2i((i + disp), (j +disp));
						glVertex2i(i,(j +disp));
					glEnd();
				}	

				else if(singleBlockColour[i][j] ==  3){
					//cellColour[x][y]= colourCode;
					glColor3f(1.0, 0.0, 1.0);
					glBegin(GL_POLYGON);
						glVertex2i(i,j);
						glVertex2i((i+disp),j);
						glVertex2i((i + disp), (j +disp));
						glVertex2i(i,(j +disp));
					glEnd();
				}

				else if(singleBlockColour[i][j] ==  4){
					//cellColour[x][y]= colourCode;
					glColor3f(1.0, 1.0, 0.0);
					glBegin(GL_POLYGON);
						glVertex2i(i,j);
						glVertex2i((i+disp),j);
						glVertex2i((i + disp), (j +disp));
						glVertex2i(i,(j +disp));
					glEnd();
				}

				else if(singleBlockColour[i][j] == 5){
					//cellColour[x][y]= colourCode;
					glColor3f(1.0, 0.0, 0.0);
					glBegin(GL_POLYGON);
						glVertex2i(i,j);
						glVertex2i((i+disp),j);
						glVertex2i((i + disp), (j +disp));
						glVertex2i(i,(j +disp));
					glEnd();
				}
			}
		}
	}

}

void Game::addBlockToBlockStore(){
	for(int j=0;j < YVAL; j++){
		for(int i=0;i < XVAL; i++){
			blockStore[i][j] = blockStore[i][j]+singleBlock[i][j];
		}
	}	
}
void Game::updateSingleBlock(Block currentBlock){
	//get the block info
	int row = currentBlock.getRow();
	int col = currentBlock.getCol();
	int rotation = currentBlock.getRotation();
	int shapeValue = currentBlock.getShape();
	int blockColour1 = currentBlock.getColour1();
	int blockColour2 = currentBlock.getColour2();
	int blockColour3 = currentBlock.getColour3();
	int blockColour4 = currentBlock.getColour4();
	//set all elements to be all zeroes
	clearSingleBlock();
	switch(shapeValue){

	//1=T, 2=L, 3 = otherL, 4 = S, 5 = Z, 6 = I

	case 1: //T shape
		if(rotation == 1){
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col-1,row,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col,row-1,shapeValue, blockColour4);
		}
		else if(rotation == 2){
			// this shape   |-
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col,row-1,shapeValue, blockColour4);
		}
		else if(rotation == 3){
			// this shape   _|_
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col-1,row,shapeValue, blockColour3);
			setSingleBlockElement(col+1,row,shapeValue, blockColour4);
		}
		else{
			// this shape   -|
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col-1,row,shapeValue, blockColour2);
			setSingleBlockElement(col,row+1,shapeValue, blockColour3);
			setSingleBlockElement(col,row-1,shapeValue, blockColour4);
		}
		break;
	case 2: //L shape
		if(rotation == 1){
			//this shape |_
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col,row-1,shapeValue, blockColour3);
			setSingleBlockElement(col+1,row-1,shapeValue, blockColour4);
		}
		else if(rotation == 2){
			// this shape   ___|
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col-1,row,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col+1,row+1,shapeValue, blockColour4);
		}
		else if(rotation == 3){
			// this shape  ''| 
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col,row-1,shapeValue, blockColour3);
			setSingleBlockElement(col-1,row+1,shapeValue, blockColour4);
		}
		else{
			// this shape   |'''''
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col-1,row,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col-1,row-1,shapeValue, blockColour4);
		}
		break;
	case 3: //Other L shape  
		if(rotation == 1){
			//this shape _|
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col,row-1,shapeValue, blockColour3);
			setSingleBlockElement(col-1,row-1,shapeValue, blockColour4);
		}
		else if(rotation == 2){
			// this shape   '''|
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col-1,row,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col+1,row-1,shapeValue, blockColour4);
		}
		else if(rotation == 3){
			// this shape  |' 
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col,row-1,shapeValue, blockColour3);
			setSingleBlockElement(col+1,row+1,shapeValue, blockColour4);
		}
		else{
			// this shape   |___
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col-1,row,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col-1,row+1,shapeValue, blockColour4);
		}
		break;
	case 4: //S shape
		if(rotation == 1){
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col+1,row,shapeValue, blockColour2);
			setSingleBlockElement(col,row-1,shapeValue, blockColour3);
			setSingleBlockElement(col-1,row-1,shapeValue, blockColour4);
		}
		else if(rotation == 2){
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col+1,row-1,shapeValue, blockColour4);
		}
		else if(rotation == 3)
		{
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row+1,shapeValue, blockColour3);
			setSingleBlockElement(col-1,row,shapeValue, blockColour4);

		}
		else 
		{
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col-1,row,shapeValue, blockColour2);
			setSingleBlockElement(col-1,row+1,shapeValue, blockColour3);
			setSingleBlockElement(col,row-1,shapeValue, blockColour4);	
		}
		break;
	case 5: //Z shape
		if(rotation == 1){
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col-1,row,shapeValue, blockColour2);
			setSingleBlockElement(col,row-1,shapeValue, blockColour3);
			setSingleBlockElement(col+1,row-1,shapeValue, blockColour4);
		}
		else if(rotation == 2){
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row-1,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col+1,row+1,shapeValue, blockColour4);
		}
		else if(rotation == 3)
		{
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col+1,row,shapeValue, blockColour3);
			setSingleBlockElement(col-1,row+1,shapeValue, blockColour4);

		}
		else 
		{
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col-1,row,shapeValue, blockColour3);
			setSingleBlockElement(col-1,row-1,shapeValue, blockColour4);	
		}
		break;
	case 6: //I shape
		if((rotation % 2) == 0)
		{
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col,row+1,shapeValue, blockColour2);
			setSingleBlockElement(col,row-1,shapeValue, blockColour3);
			setSingleBlockElement(col,row-2,shapeValue, blockColour4);
		}
		else
		{
			setSingleBlockElement(col,row,shapeValue, blockColour1);
			setSingleBlockElement(col+1,row,shapeValue, blockColour2);
			setSingleBlockElement(col-1,row,shapeValue, blockColour3);
			setSingleBlockElement(col-2,row,shapeValue, blockColour4);
		}
		break;
	default:
		break;
	}
		
}





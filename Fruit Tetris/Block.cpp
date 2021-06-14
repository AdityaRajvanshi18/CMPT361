/*
	Block.cpp
	Aditya Rajvanshi
*/

//#include "Constants.h"
#include "Block.h"
#include <GL/glut.h>
#include <cstdlib>

//Constructor
Block::Block(){
	setNextShape();
	resetBlock();
}


//Getter and Setters
int Block::getNextShape(){
	return nextShape;
}
int Block::getRow(){
	return rowY;
}
int Block::getCol(){
	return colX;
}
int Block::getRotation(){
	return curRot;	
}
int Block::getShape(){
	return shape;
}

int Block::getColour1(){
	return curColour1;
}

int Block::getColour2(){
	return curColour2;
}

int Block::getColour3(){
	return curColour3;
}

int Block::getColour4(){
	return curColour4;
}

int Block::setRandShape(){
	//1=T, 2=L, 3 = otherL, 4 = S, 5 = Z, 6 = I
	int randShape = rand()%6;
	randShape++;
	return randShape;
}

int Block::setRandColour(){
	int randColour = rand()%5;
	randColour++;
	return randColour;
}
void Block::setNextShape(){
	nextShape = setRandShape();
	curColour1 = setRandColour();
	curColour2 = setRandColour();
	curColour3 = setRandColour();
	curColour4 = setRandColour();
}
	
void Block::resetBlock(){
	rowY = 19;
	colX = 5;
	curRot = 1;
	shape = nextShape;
	setNextShape();
}
void Block::moveBottom(bool isDownArrowPressed){
	if(!isCollide(BOTTOM)){
		rowY--;
	}
}
void Block::moveRight(){
	if(!isCollide(RIGHT)){
		colX++;
	}
}
void Block::moveLeft(){
	if(!isCollide(LEFT)){
		colX--;
	}
}
void Block::rotateShape(){
	if(curRot == 4){
		curRot = 1;
	}
	else{
		curRot++;
	}
}

bool Block::isCollide(Sides side){
	switch(side){
		case LEFT:
			if(colX <= 0){
				return true;
			}
		break;
		case RIGHT:
			if(colX >= 10){
				return true;
			}
		break;
		case BOTTOM:
			if(rowY <= 0){
				return true;
			}
		break;
	}
	return false;
}






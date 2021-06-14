/*
	Block.h
	Aditya Rajvanshi
*/
#pragma once
#include <cstdlib>
#include "Constants.h"

class Block{
private:
	
	int rowY;
	int colX;
	int curRot;
	int shape;
	int nextShape;
	int curColour1;
	int curColour2;
	int curColour3;
	int curColour4;
public:
	//Constructor
	Block();	

	//Getters and Setters
	int getNextShape();
	int getRow();
	int getCol();
	int getRotation();
	int getShape();

	int getColour1();
	int getColour2();
	int getColour3();
	int getColour4();

	int setRandShape();
	int setRandColour();	
	void setNextShape();

	void resetBlock();
	void moveBottom(bool isDownArrowPressed);
	void moveRight();
	void moveLeft();
	void rotateShape();
	bool isCollide(Sides side);
};

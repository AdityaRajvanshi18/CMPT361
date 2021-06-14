/*
	Board.h
	Aditya Rajvanshi
*/
#pragma once
#include <glm/glm.hpp>
#include "Constants.h"

using namespace glm;

class Board{

private:
	int board[10][20];
	bool cellOccupied[10][20];
	int cellColour[10][20];
public:
	//Constructor
	Board();	

	void drawBoard();
	//Getters and Setters
	//get whether cell[x][y] is occupied
	bool getCellOccupied(int x, int y); 

	//return true if cell[x][y] occupation set successfully
	bool setCellOccupied(int x, int y); 
	//Set Board Colour
	void setCellColour(int x, int y, int colourCode); 

	//Render colours and cell occupied correctly
	//void updateBoard();	
	
};



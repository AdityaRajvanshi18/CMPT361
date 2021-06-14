/*
	Game.h
	Aditya Rajvanshi		
*/
#pragma once
#include "Constants.h"
#include "Block.h"
#include <stdio.h>

class Game{

private:
	int singleBlock[10][20];
	int blockStore[10][20];
	int singleBlockColour[10][20];
	int blockStoreColour;
public:
	Game();
	
	void clearSingleBlock();
	void clearBlockStore();

	void setSingleBlockElement(int col, int row, int value, int colourCode);
	int getBlockStoreElement(int i, int j);

	bool isBlockTouchBlock(Sides side);
	bool isBlockStoreFull();	
	bool isCompleteRow(int row);
	bool isRowEmpty(int row);
	void shiftBlocksDown();
	void drawBlockStore();
	void addBlockToBlockStore();
	void updateSingleBlock(Block currentBlock);

};

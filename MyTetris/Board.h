/*
	Filename: Board.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate a board of TETRIS GAME
	Comments: 
	Attribution: Based in part of tutorial at gametuto.com - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _BOARD
#define _BOARD

//Library dependencies

//Classes dependencies
#include "Pieces.h"

//Definitions
const int BOARD_LINE_WIDTH = 12;		// Width of each of the two lines that delimit the board (pixels)
const int BLOCK_SIZE = 30;				// Width and Height of each block of a piece (pixels)
const int BOARD_POSITION = 400;			// Center position of the board from the left of the screen (pixels)
const int BOARD_WIDTH = 10;				// Board width in blocks 
const int BOARD_HEIGHT = 16;			// Board height in blocks

class Board
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	Board(Pieces *pieces, int screenheight) : mScreenHeight(screenheight), mPieces(pieces)
	{	
		//Init the board blocks with free positions
		_initBoard();
	}
	~Board()
	{}
	
	//----- GET/SET FUNCTIONS -----
	int GetXPosInPixels(int pos);
	int GetYPosInPixels(int pos);
	//----- OTHER FUNCTIONS -----
	bool IsFreeBlock(int x, int y);
	bool IsPossibleMovement(int x, int y);
	void StorePiece(int x, int y);
	void RotateCurrentPiece(bool cw);
	int DeletePossibleLines();
	bool IsGameOver();
	void Restart();

protected:
	//----- INTERNAL VARIABLES -----
	enum { POS_FREE, POS_FILLED };			// Position free of occupied
	int mBoard [BOARD_WIDTH][BOARD_HEIGHT];	// Board that contains the pieces
	Pieces *mPieces;						// Class wrapper of pieces data and management of current piece
	int mScreenHeight;						//

	//----- INTERNAL FUNCTIONS -----
	void _initBoard();
	void _deleteLine (int y);
};

#endif
/*
	Filename: Board.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate a board of TETRIS GAME
	Comments: 
	Attribution: Based in part of tutorial at gametuto.com - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "Board.h"
					
//Init the board blocks with free positions
void Board::_initBoard()
{
	for (int i = 0; i < BOARD_WIDTH; i++)
		for (int j = 0; j < BOARD_HEIGHT; j++)
			mBoard[i][j] = POS_FREE;
}

//Reset board to initial state
void Board::Restart()
{
	_initBoard();
}
								
//Store a piece in the board by filling the blocks
void Board::StorePiece (int x, int y)
{
	// Store each block of the piece into the board
	for (int i1 = x, i2 = 0; i1 < x + mPieces->GetMatrixOrder(); i1++, i2++)
	{
		for (int j1 = y, j2 = 0; j1 < y + mPieces->GetMatrixOrder(); j1++, j2++)
		{	
			// Store only the blocks of the piece that are not holes
			if (mPieces->GetBlockType (i2, j2) != Pieces::FREE)		
				mBoard[i1][j1] = POS_FILLED;	
		}
	}
}

//Rotate piece inside board
void Board::RotateCurrentPiece(bool cw)
{
	//Transfer rotation command to current piece
	mPieces->RotateCurrentPiece(cw);
}

			
//Check if the game is over becase a piece have achived the upper position
bool Board::IsGameOver()
{
	//If the first line has blocks, then, game over
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		if (mBoard[i][0] == POS_FILLED) 
		{
			return true;
		}
	}

	return false;
}
								
//Delete a line of the board by moving all above lines down
void Board::_deleteLine (int y)
{
	// Moves all the upper lines one row down
	for (int j = y; j > 0; j--)
	{
		for (int i = 0; i < BOARD_WIDTH; i++)
		{
			mBoard[i][j] = mBoard[i][j-1];
		}
	}	
}
								
//Delete all the lines that should be removed - returns number of deleted lines
int Board::DeletePossibleLines ()
{
	int deletedlines = 0;

	for (int j = 0; j < BOARD_HEIGHT; j++)
	{
		int i = 0;
		while (i < BOARD_WIDTH)
		{
			if (mBoard[i][j] != POS_FILLED) break;
			i++;
		}

		if (i == BOARD_WIDTH) 
		{
			_deleteLine (j);
			deletedlines++;
		}
	}

	return deletedlines;
}


								
//Returns 1 (true) if this block of the board is empty, 0 if it is filled
bool Board::IsFreeBlock (int x, int y)
{
	if (mBoard [x][y] == POS_FREE) 
		return true; 
	else 
		return false;
}

//Returns the horizontal position (in pixels) of the block given like parameter
int Board::GetXPosInPixels (int pos)
{
	return  ( ( BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) ) + (pos * BLOCK_SIZE) );
}
					
//Returns the vertical position (in pixels) of the block given like parameter
int Board::GetYPosInPixels (int pos)
{
	return ( (mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT)) + (pos * BLOCK_SIZE) );
}
									
//Check if the piece can be stored at this position without any collision
//Returns true if the movement is  possible, false if it not possible
bool Board::IsPossibleMovement (int x, int y)
{
	//Check collissions
	for (int i1 = x, i2 = 0; i1 < x + mPieces->GetMatrixOrder(); i1++, i2++)
	{
		for (int j1 = y, j2 = 0; j1 < y + mPieces->GetMatrixOrder(); j1++, j2++)
		{	
			// Check if the piece is outside the limits of the board
			if (	i1 < 0 			|| 
				i1 > BOARD_WIDTH  - 1	||
				j1 > BOARD_HEIGHT - 1)
			{
				if (mPieces->GetBlockType (i2, j2) != 0)
					return false;		
			}

			// Check if the piece have collisioned with a block already stored in the map
			if (j1 >= 0)	
			{
				if ((mPieces->GetBlockType (i2, j2) != 0) &&
					(!IsFreeBlock (i1, j1))	)
					return false;
			}
		}
	}

	// No collision
	return true;
}

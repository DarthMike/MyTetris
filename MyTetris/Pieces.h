/*
	Filename: Pieces.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate all pieces of TETRIS game
	Comments: THIS CLASS ASSUMES RANDOM NUMBER GENERATOR WAS SEED CORRECTLY
	Attribution: Based in part of tutorial at gametuto.com - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _PIECES
#define _PIECES

//Library dependencies

//Classes dependencies
#include "Generic2DMatrix.h"
#include "General_Resources.h"
#include "Math.h"

class Pieces
{
public:
	typedef std::map<int, Generic2DMatrix<int>*> PiecesMap;
	enum Blocktype {FREE,OCCUPIED,PIVOT};
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	Pieces() : mMatrixOrder(5), mNumberofPieces(7), mCurrentPiece(mMatrixOrder,mMatrixOrder), mNextPiece(mMatrixOrder,mMatrixOrder)
	{
		//Initialize types of pieces
		_initPieces();
	}
	~Pieces()
	{}

	//----- GET/SET FUNCTIONS -----
	int GetNumberofPieces() { return mNumberofPieces;}
	int GetMatrixOrder() {return mMatrixOrder;}
	Blocktype GetBlockType(int x, int y);
	Blocktype GetNextPieceBlockType(int x, int y);
	int GetXInitialPosition ();  
	int GetYInitialPosition (); 
	
	//----- OTHER FUNCTIONS -----
	bool CreateNewPiece();					//Create a random new piece / create it in buffer of incoming pieces
	void RotateCurrentPiece(bool cw);		//Rotate current piece

protected:
	//----- INTERNAL VARIABLES -----
	PiecesMap mPiecesTypes;					//Container of models for pieces MAP: INT (0 - mNumberofPieces) -->> MATRIX2D				
	const int mNumberofPieces;				//Number of pieces created
	const int mMatrixOrder;					//Matrix Order to create pieces
	Generic2DMatrix<int> mCurrentPiece;					//Current piece tracked
	Generic2DMatrix<int> mNextPiece;					//Next piece to track
	
	//----- INTERNAL FUNCTIONS -----
	void _initPieces();
};

#endif

/*
	Filename: Pieces.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate all pieces of TETRIS game
	Comments: THIS CLASS ASSUMES RANDOM NUMBER GENERATOR WAS SEED CORRECTLY
	Attribution: Based in part of tutorial at gametuto.com - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "Pieces.h"

//----- GET/SET FUNCTIONS -----------------------------------------------
//Returns the mask or block type inside current piece given a coordinate
Pieces::Blocktype Pieces::GetBlockType(int x, int y)
{
	//Assert correct coordinates
	assert(x<mMatrixOrder && y<mMatrixOrder);
	
	//Return value
	Blocktype positiontype;

	//Conversion from int value to BLOCKTYPE value
	switch(mCurrentPiece.GetValue(x,y))
	{
	case(0):
		{
			positiontype = FREE;
		}
		break;
	
	case(1):
		{
			positiontype = OCCUPIED;
		}
		break;

	case(2):
		{
			positiontype = PIVOT;
		}
		break;

	default:
		{	
			positiontype = FREE;
		}
		break;
	}

	return(positiontype);
}

//Returns the mask or block type inside current piece given a coordinate
Pieces::Blocktype Pieces::GetNextPieceBlockType(int x, int y)
{
	//Assert correct coordinates
	assert(x<mMatrixOrder && y<mMatrixOrder);
	
	//Return value
	Blocktype positiontype;

	//Conversion from int value to BLOCKTYPE value
	switch(mNextPiece.GetValue(x,y))
	{
	case(0):
		{
			positiontype = FREE;
		}
		break;
	
	case(1):
		{
			positiontype = OCCUPIED;
		}
		break;

	case(2):
		{
			positiontype = PIVOT;
		}
		break;

	default:
		{	
			positiontype = FREE;
		}
		break;
	}

	return(positiontype);
}

//Compute initial offset to put piece in upper-left edge of matrix
int Pieces::GetXInitialPosition ()
{
	//For current piece rotation, an offset has to be calculated
	//FOR X ->>> FIND MIDDLE OF PIECE!!

	//For all types of pieces, offset in x is allways -2
	return(-2);
}

//Compute initial offset to put piece in upper-left edge of matrix
int Pieces::GetYInitialPosition ()
{
	//For current piece rotation, an offset has to be calculated
	int currentoffset = 2;

	//LOOP - GO THROUGH ALL ROWS
	for(int i = 0; i<mMatrixOrder; i++)
	{
		//LOOP - GO THROUGH ALL COLUMNS
		for(int j = 0; j<mMatrixOrder; j++)
		{
			if(
				mCurrentPiece.GetValue(i,j) == Pieces::OCCUPIED
				||
				mCurrentPiece.GetValue(i,j) == Pieces::PIVOT
				)
			{
				//Update current offset if found smaller
				if(j < currentoffset)
					currentoffset = j;
				//Search next column
				break;
			}
		}//LOOP END - GO THROUGH ALL ROWS
	}//LOOP END - GO THROUGH ALL COLUMNS

	//Return distance from center (not from lateral)
	return(-currentoffset);
}
	

//-----------------------------------------------------------------------
//----- OTHER FUNCTIONS -------------------------------------------------
//Create a random new piece / overwrite current if existing previously
bool Pieces::CreateNewPiece()					
{
	//A new type is generated randomly from math utility class
	int newtype = SingletonMath::Instance()->NewRandom(1,mNumberofPieces);
	int rotations = SingletonMath::Instance()->NewRandom(0,3);
	
	//Search and check type existed
	PiecesMap::iterator it = mPiecesTypes.find(newtype);
	if(it != mPiecesTypes.end())
	{	
		//IF - Check if current piece is not zero (first generation of piece)
		if(!mCurrentPiece.IsZero())
		{
			//NOTE: Use overloaded = operator to copy contents of matrices
			mCurrentPiece = mNextPiece;  //Previously generated piece is new one
			mNextPiece = *(it->second);  //Newly generated piece is next to come
		}
		else //ELSE - Check if current piece is not zero (first generation of piece)
		{
			mNextPiece = *(it->second);  //Newly generated piece is next to come
			//Generate another piece for current one
			newtype = SingletonMath::Instance()->NewRandom(1,mNumberofPieces);
			rotations = SingletonMath::Instance()->NewRandom(0,3);
			it = mPiecesTypes.find(newtype);
			if(it != mPiecesTypes.end())
			{
				mCurrentPiece = *(it->second); // Copy contents of piece to current
			}

		}//IF END- Check if current piece is not zero (first generation of piece)
			
		//Rotate randomly new piece 
		for(int i = 0;i<=rotations;i++)
		{
			//Rotate "rotations" times matrix
			mNextPiece.Rotate(true);
		}

		//Operation successful
		return true;
	}

	//Operation unsuccessful
	return false;
}

//Rotate current piece
void Pieces::RotateCurrentPiece(bool cw)				
{
	mCurrentPiece.Rotate(cw);
}

//Init values for piece type
void Pieces::_initPieces()
{
	//Local variables to store name-type mapping
	int type = 0;
	Generic2DMatrix<int> *matrix = NULL;

	//Local variables
	int i,j;

	//SQUARE TEMPLATE
	int square [5][5] =     
	{0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 2, 1, 0,
    0, 0, 1, 1, 0,
	0, 0, 0, 0, 0};
	
	type++;
	matrix = new Generic2DMatrix<int>(5,5);
	//LOOP - COPY TEMPLATE DATA
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			//Copy matrix template to piece data
			matrix->SetValue(i,j,square[i][j]);
		}
	}//LOOP END - COPY TEMPLATE DATA

	//Insert template data to container
	mPiecesTypes.insert(std::make_pair<int,Generic2DMatrix<int>*>(type,matrix));

	//L TEMPLATE
	int L [5][5] =     
	{0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 2, 1, 0,
    0, 0, 0, 0, 0,
	0, 0, 0, 0, 0};
	
	type++;
	matrix = new Generic2DMatrix<int>(5,5);

	//LOOP - COPY TEMPLATE DATA
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			//Copy matrix template to piece data
			matrix->SetValue(i,j,L[i][j]);
		}
	}//LOOP END - COPY TEMPLATE DATA

	//Insert template data to container
	mPiecesTypes.insert(std::make_pair<int,Generic2DMatrix<int>*>(type,matrix));

	int Linv [5][5] =     
	{0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 2, 0, 0,
    0, 0, 0, 0, 0,
	0, 0, 0, 0, 0};
	
	type++;
	matrix = new Generic2DMatrix<int>(5,5);

	//LOOP - COPY TEMPLATE DATA
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			//Copy matrix template to piece data
			matrix->SetValue(i,j,Linv[i][j]);
		}
	}//LOOP END - COPY TEMPLATE DATA

	//Insert template data to container
	mPiecesTypes.insert(std::make_pair<int,Generic2DMatrix<int>*>(type,matrix));

	int I [5][5] =     
	{0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 1, 2, 1, 0,
    0, 0, 0, 0, 0,
	0, 0, 0, 0, 0};

	type++;
	matrix = new Generic2DMatrix<int>(5,5);

	//LOOP - COPY TEMPLATE DATA
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			//Copy matrix template to piece data
			matrix->SetValue(i,j,I[i][j]);
		}
	}//LOOP END - COPY TEMPLATE DATA

	//Insert template data to container
	mPiecesTypes.insert(std::make_pair<int,Generic2DMatrix<int>*>(type,matrix));

	int T [5][5] =     
	{0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 1, 2, 1, 0,
    0, 0, 1, 0, 0,
	0, 0, 0, 0, 0};

	type++;
	matrix = new Generic2DMatrix<int>(5,5);

	//LOOP - COPY TEMPLATE DATA
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			//Copy matrix template to piece data
			matrix->SetValue(i,j,T[i][j]);
		}
	}//LOOP END - COPY TEMPLATE DATA

	//Insert template data to container
	mPiecesTypes.insert(std::make_pair<int,Generic2DMatrix<int>*>(type,matrix));

	int S [5][5] =     
	{0, 0, 0, 0, 0,
    0, 0, 1, 1, 0,
    0, 1, 2, 0, 0,
    0, 0, 0, 0, 0,
	0, 0, 0, 0, 0};

	type++;
	matrix = new Generic2DMatrix<int>(5,5);

	//LOOP - COPY TEMPLATE DATA
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			//Copy matrix template to piece data
			matrix->SetValue(i,j,S[i][j]);
		}
	}//LOOP END - COPY TEMPLATE DATA

	//Insert template data to container
	mPiecesTypes.insert(std::make_pair<int,Generic2DMatrix<int>*>(type,matrix));

	int Sinv [5][5] =     
	{0, 0, 0, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 2, 1, 0,
    0, 0, 0, 0, 0,
	0, 0, 0, 0, 0};

	type++;
	matrix = new Generic2DMatrix<int>(5,5);

	//LOOP - COPY TEMPLATE DATA
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			//Copy matrix template to piece data
			matrix->SetValue(i,j,Sinv[i][j]);
		}
	}//LOOP END - COPY TEMPLATE DATA

	//Insert template data to container
	mPiecesTypes.insert(std::make_pair<int,Generic2DMatrix<int>*>(type,matrix));
	
}


/*
	Filename: Game.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class which makes game logic for a TETRIS game
	Comments: 
	Attribution: Based in part of tutorial at gametuto.com - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "TetrisGame.h"

//Updates game state and returns if user lost , or game should finish 
void TetrisGame::UpdateGameState(float dt)
{
	//Local variables
	bool exit = false;
	
	//Check if its first program run
	if(mFirstExecution)
	{
		//It will not be
		mFirstExecution = false;
		//Reset internal timers
		mMainTimer.Start();
		mSpeedChangeTimer.Start();
		//Reset piece speed
		mPieceSpeed = INITIALPIECESPEED;
		mLastPieceSpeed = INITIALPIECESPEED;
		//Reset score
		mScore = 0;
		mScoreTicks = 0;
		mLines = 0;	
		mNumPieces = 0;
	}

	//Check if speed of piece should be incremented (when not making automovements)
	if(!mAutoMovements)
	{
		if(mSpeedChangeTimer.GetTicks() > INCREMENTSPEEDTIME * 1000)
		{
			//Reset count
			mSpeedChangeTimer.Start();
			//Make the actual change of speed
			_incrementPieceSpeed();
			//Increment points given to player per piece and per line
			mScoreTicks++;
		}
	}
	
	//Execute movement of piece (if needed)
	//IF - Piece inside board should move?
	if ((mMainTimer.GetTicks() * mPieceSpeed ) > 1000)
	{
		mMainTimer.Start(); //Reset time count
			//IF - Check if piece can move inside board
		if (mBoard->IsPossibleMovement(mPosX, mPosY + 1))
		{
			//Piece can move - make it fall
			mPosY++;
		}
		else //ELSE - Piece cant move inside board (Y axis)
		{
			_storePiece(mPosX,mPosY);

			int deletedlines = mBoard->DeletePossibleLines();
			
			//Play sound if a line was deleted
			if( deletedlines != 0)
			{
				//(Randomize erased line sounds)
				if(SingletonMath::Instance()->NewRandom(0,100) > 50)
					SingletonSoundMgr::Instance()->PlayAction("LineErased1");
				else
					SingletonSoundMgr::Instance()->PlayAction("LineErased2");
				//Increment lines made
				mLines+=deletedlines;
			}

			//Give points to player 
					  //Score per piece (incremented by speed)
			mScore += (SCOREPERPIECE + mScoreTicks * SCOREINCREMENT) 
				  +//Score per all rows deleted (incremented by speed)
					  (deletedlines* (SCOREPERROW + mScoreTicks * SCOREINCREMENT));

			//IF - END OF GAME?
			if (!mBoard->IsGameOver())
			{
				//Create new piece
				_createNewPiece();	
			}
			else
			{
				//Stop updating
				mMainTimer.Stop();
				mSpeedChangeTimer.Stop();
				//Update scores file
				mScoresFile.AddScore((float)mScore);
			}//END IF - END OF GAME?
		}//END IF - Check if piece can move inside board
	}//END IF - Piece inside board should move?
}

//Restart game
void TetrisGame::Restart()
{
	//Init game again
	_initGame();
}

//Pause/unpause game
void TetrisGame::TogglePause()
{
	if(mPause)
	{
		mMainTimer.Unpause();
		mSpeedChangeTimer.Unpause();
		mPause = false;
		SingletonLogMgr::Instance()->AddNewLine("TetrisGame","Game resumed...",DEBUG);
	}
	else
	{
		mMainTimer.Pause();
		mSpeedChangeTimer.Pause();
		mPause = true;
		SingletonLogMgr::Instance()->AddNewLine("TetrisGame","Game paused...",DEBUG);
	}
}
//Move piece around board
void TetrisGame::MovePieceX(bool Positive)
{
	//Movements possible if there is no automatic actions
	if(!mAutoMovements && !mPause)
	{
		//Positive means (+ increment or - increment of position)
		//Move checking conditions for collision
		if(Positive && mBoard->IsPossibleMovement(mPosX + 1,mPosY))
		{
			mPosX++;
		}
		else if (!Positive && mBoard->IsPossibleMovement(mPosX - 1,mPosY))
		{
			mPosX--;
		}
	}
}

//Move piece around board
void TetrisGame::MovePieceY()
{
	//Movements possible if there is no automatic actions
	if(!mAutoMovements && !mPause)
	{
		//Check if movement is possible
		if (mBoard->IsPossibleMovement(mPosX,mPosY + 1))
		{
			mPosY++;
		}
	}
}

//Rotate current piece
void TetrisGame::RotateCurrentPiece(bool cw)
{
	//Movements possible if there is no automatic actions
	if(!mAutoMovements && !mPause)
	{
		mBoard->RotateCurrentPiece(cw);

		if (!mBoard->IsPossibleMovement (mPosX, mPosY))
		{
			//Movement not possible -> rotate back
			mBoard->RotateCurrentPiece(!cw);
		}
	}
}	
//Moves piece down to first place to collision
void TetrisGame::MovePieceDown()
{
	if(!mPause)
	{
		mPieceSpeed = PIECEFALLSPEED;	//2 BLOCKS PER SECOND TO MAKE PIECE GO DOWN FAST
		mAutoMovements = true;	//For this movement, user input is disabled
	}
}


//Request for new random piece
void TetrisGame::_createNewPiece()
{
	//Reset piece speed
	mPieceSpeed = mLastPieceSpeed;
	//Reset automovements if there were some
	mAutoMovements = false;
	//Select surface for piece randomly
	int newrandom = SingletonMath::Instance()->NewRandom(1,4);
	if(newrandom == 1)
		mCurrentPieceSurf = &mSurfBlockNormal;
	else if(newrandom == 2)
		mCurrentPieceSurf = &mSurfBlockNormal2;
	else
		mCurrentPieceSurf = &mSurfBlockNormal3;

	//Create new pieces
	if(mPieces->CreateNewPiece())
		SingletonLogMgr::Instance()->AddNewLine("TetrisGame","New Piece created",DEBUG);
	else
		SingletonLogMgr::Instance()->AddNewLine("TetrisGame","Piece could not be created",EXCEPTION);

	//Init variables of pieces positions
	mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition();  
	mPosY = mPieces->GetYInitialPosition();


}


//Drawing of whole scene
void TetrisGame::DrawScene ()
{
	//Every loop:
	//Draw background and UI
		//TODO: Implement UI
	//Draw static board (overwrites piece
		_drawBoard();
	//Draw movable piece (overwriting static board)
		_drawPiece (mPosX, mPosY);	
	//Draw piece to come
		_drawNextPiece ();		
}

//Increment falling speed of Piece
void TetrisGame::_incrementPieceSpeed()
{
	mPieceSpeed += mPieceSpeed * PIECESPEEDINCREMENT;
	mLastPieceSpeed = mPieceSpeed;
}

//Init function 
void TetrisGame::_initGame()
{
	//Reset contents of board
	mBoard->Restart();

	//Remember game starts again
	mFirstExecution = true;

	//Call creation of 1 new piece to start
	_createNewPiece();
}

//Load resources to display elements of game
void TetrisGame::_loadResources()
{
	//Local variables
	bool fail = false;
	IND_Image loadedimage;

	//Get indie lib manager pointer
	IndieLibManager* pIlib = SingletonIndieLib::Instance();

	if(!pIlib->ImageManager->Add(&loadedimage,".\\GameData\\Image\\Block1.png"))
		fail = true;
	//Scale image to real block size
	loadedimage.Scale(BLOCK_SIZE,BLOCK_SIZE);
	pIlib->SurfaceManager->Add(&mSurfBlockBoard,&loadedimage,IND_ALPHA,IND_32);
	pIlib->ImageManager->Delete(&loadedimage);

	if(!pIlib->ImageManager->Add(&loadedimage,".\\GameData\\Image\\Block5.png"))
		fail = true;
	//Scale image to real block size
	loadedimage.Scale(BLOCK_SIZE,BLOCK_SIZE);
	pIlib->SurfaceManager->Add(&mSurfEmptyBoard,&loadedimage,IND_ALPHA,IND_32);
	pIlib->ImageManager->Delete(&loadedimage);

	if(!pIlib->ImageManager->Add(&loadedimage,".\\GameData\\Image\\Block2.png"))
		fail = true;
	//Scale image to real block size
	loadedimage.Scale(BLOCK_SIZE,BLOCK_SIZE);
	pIlib->SurfaceManager->Add(&mSurfBlockNormal,&loadedimage,IND_ALPHA,IND_32);
	pIlib->ImageManager->Delete(&loadedimage);
	
	if(!pIlib->ImageManager->Add(&loadedimage,".\\GameData\\Image\\Block3.png"))
		fail = true;
	//Scale image to real block size
	loadedimage.Scale(BLOCK_SIZE,BLOCK_SIZE);
	pIlib->SurfaceManager->Add(&mSurfBlockNormal2,&loadedimage,IND_ALPHA,IND_32);
	pIlib->ImageManager->Delete(&loadedimage);

	if(!pIlib->ImageManager->Add(&loadedimage,".\\GameData\\Image\\Block6.png"))
		fail = true;
	//Scale image to real block size
	loadedimage.Scale(BLOCK_SIZE,BLOCK_SIZE);
	pIlib->SurfaceManager->Add(&mSurfBlockNormal3,&loadedimage,IND_ALPHA,IND_32);
	pIlib->ImageManager->Delete(&loadedimage);

	if(!pIlib->ImageManager->Add(&loadedimage,".\\GameData\\Image\\Block4.png"))
		fail = true;
	//Scale image to real block size
	loadedimage.Scale(BLOCK_SIZE,BLOCK_SIZE);
	pIlib->SurfaceManager->Add(&mSurfBlockNext,&loadedimage,IND_ALPHA,IND_32);
	pIlib->ImageManager->Delete(&loadedimage);

	if (fail)
		throw(GenericException("Unable to add image",GenericException::GRAPHICS_ERROR));

	//Set initial display of board (empty)
	for(int i = 0;i<BOARD_WIDTH;i++)
	{
		for(int j =0;j<BOARD_HEIGHT;j++)
		{
			//Calculate where in screen will it be drawn
			int xcoordpixels = mBoard->GetXPosInPixels(i);
			int ycoordpixels = mBoard->GetYPosInPixels(j);
		
			//Create entity for this position
			GraphicBoardTile* newtile = new GraphicBoardTile(xcoordpixels,ycoordpixels,20);
			//Store in container
			mBoardGfxMap.SetValue(i,j,newtile);

			//Debug
			//mBoardGfxMap.GetValue(i,j)->SetSurface(&mSurfEmptyBoard);
		}
	}

	int piecesize = mPieces->GetMatrixOrder();

	//Set initial display of next piece (empty)
	for(int i = 0;i<mPieces->GetMatrixOrder();i++)
	{
		for(int j =0;j<mPieces->GetMatrixOrder();j++)
		{
			//Calculate where in screen will it be drawn
			int xcoordpixels = mNextPosX + (i * BLOCK_SIZE);
			int ycoordpixels = mNextPosY + (j * BLOCK_SIZE);
			
			//Create entity for this position
			GraphicBoardTile* newtile= new GraphicBoardTile(xcoordpixels,ycoordpixels,20);
			//Store in container
			mNextPieceGfxMap.SetValue(i,j,newtile);

			//Debug
			//mNextPieceGfxMap.GetValue(i,j)->SetSurface(&mSurfEmptyBoard);
		}
	}
}

//Drawing of a piece
void TetrisGame::_drawPiece (int x, int y)
{
	//Draw piece moving inside board
	//Get piece size
	int piecesize = mPieces->GetMatrixOrder();

	//NOTE: Double loop
	// Coord of piece (i,j), coord of board (k,l)
	//LOOP - Board pieces (x coord)
	for(int i = 0,k = x;i<piecesize;i++,k++)
	{	//LOOP - Board pieces (y coord)
		for(int j = 0,l = y;j<piecesize;j++,l++)
		{
			if(l>=0 && l<BOARD_HEIGHT
			   &&
			   k>=0 && k<BOARD_WIDTH)
			{
				//Get piece type
				Pieces::Blocktype type = mPieces->GetBlockType(i,j);
				
				//Depending on type draw...
				switch(type)
				{
				case(Pieces::OCCUPIED):
				case(Pieces::PIVOT):
				{
					//OCCUPIED - Normal part
					mBoardGfxMap.GetValue(k,l)->SetSurface(mCurrentPieceSurf);
				}
				break;
				break;	
				case(Pieces::FREE):
				default:
				{
					//FREE -Dont draw anything
				}
				break;
				}
			}
		}//LOOP END
	}//LOOP END
}

//Drawing of a new piece
void TetrisGame::_drawNextPiece ()
{
	//Draw next piece preview
	//Get piece size
	int piecesize = mPieces->GetMatrixOrder();

	//LOOP - Board pieces (x coord)
	for(int i = 0;i<piecesize;i++)
	{	//LOOP - Board pieces (y coord)
		for(int j = 0;j<piecesize;j++)
		{
			//Get piece type
			Pieces::Blocktype type = mPieces->GetNextPieceBlockType(i,j);
			
			//Depending on type draw...
			switch(type)
			{
			case(Pieces::OCCUPIED):
			case(Pieces::PIVOT):
			{
				//OCCUPIED
				mNextPieceGfxMap.GetValue(i,j)->SetSurface(&mSurfBlockNext);
			}
			break;	
			case(Pieces::FREE):
			default:
			{
				//FREE - Draw empty surface
				mNextPieceGfxMap.GetValue(i,j)->SetSurface(&mSurfEmptyBoard);
			}
			break;
			}
		}//LOOP END
	}//LOOP END
}

//Drawing of board
void TetrisGame::_drawBoard ()
{
	//Draw board (static pieces)
	//LOOP - Board pieces (x coord)
	for(int i = 0;i<BOARD_WIDTH;i++)
	{	//LOOP - Board pieces (y coord)
		for(int j = 0;j<BOARD_HEIGHT;j++)
		{
			//IF - Is position free?
			if(mBoard->IsFreeBlock(i,j))
			{
				//FREE - Draw empty surface
				//i = (posx * BOARD_HEIGHT) + posy
				mBoardGfxMap.GetValue(i,j)->SetSurface(&mSurfEmptyBoard);
			}
			else //ELSE - Is position free?
			{
				//OCCUPIED - Draw block occupied surface
				//i = (posx * BOARD_HEIGHT) + posy
				mBoardGfxMap.GetValue(i,j)->SetSurface(&mSurfBlockBoard);
			}
		}//LOOP END
	}//LOOP END
}

void TetrisGame::_storePiece(int posx,int posy)
{
	//Check which position is in coordinates x-y
	mBoard->StorePiece(posx,posy);
	
	//Play sound of piece stored
	//(Randomize erased line sounds)
	if(SingletonMath::Instance()->NewRandom(0,100) > 50)
		SingletonSoundMgr::Instance()->PlayAction("PieceFall1");
	else
		SingletonSoundMgr::Instance()->PlayAction("PieceFall2");

	//Increment counter of stored pieces
	mNumPieces++;

	SingletonLogMgr::Instance()->AddNewLine("TetrisGame","Piece stored in board",DEBUG);
}

//Free dynamically allocated memory
void TetrisGame::_cleanupMemory()
{
	//Get indie lib manager pointer
	IndieLibManager* pIlib = SingletonIndieLib::Instance();

	//Board and pieces object
	if(mBoard)
		delete mBoard;
	if(mPieces)
		delete mPieces;

	pIlib->SurfaceManager->Delete(&mSurfBlockBoard);		//Indielib Images for drawing
	pIlib->SurfaceManager->Delete(&mSurfEmptyBoard);
	pIlib->SurfaceManager->Delete(&mSurfBlockNormal);
	pIlib->SurfaceManager->Delete(&mSurfBlockNormal2);
	pIlib->SurfaceManager->Delete(&mSurfBlockNext);

	//Created entities
	for(int i = 0;i<mBoardGfxMap.GetSizeX();i++)
	{
		for(int j = 0;j<mBoardGfxMap.GetSizeY();j++)
		{
			if(mBoardGfxMap.GetValue(i,j))
			{
				delete mBoardGfxMap.GetValue(i,j);
			}
		}
	}

	for(int i = 0;i<mNextPieceGfxMap.GetSizeX();i++)
	{
		for(int j = 0;j<mNextPieceGfxMap.GetSizeY();j++)
		{
			if(mNextPieceGfxMap.GetValue(i,j))
			{
				delete mNextPieceGfxMap.GetValue(i,j);
			}
		}
	}
}

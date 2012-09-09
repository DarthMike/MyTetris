/*
	Filename: Game.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class which makes game logic for a TETRIS game
	Comments: 
	Attribution: Based in part of tutorial at gametuto.com - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it
*/
#ifndef _TETRISGAME
#define _TETRISGAME

//Library dependencies

//Class dependencies
#include "General_Resources.h"
#include "Pieces.h"
#include "Board.h"
#include "GraphicBoardTile.h"
#include "HighScores.h"

//Definitions
const float INITIALPIECESPEED = 1.2f;		//Falling speed of piece, in blocks/s
const float PIECESPEEDINCREMENT = 0.05f;	//Increment of piece speed in percentage
const float PIECEFALLSPEED = 30.0f;			//Falling speed when prompted to go "down fast"
const float INCREMENTSPEEDTIME = 10.0f;		//Time (in seconds) when speed of falling piece will be incremented
const int SCOREINCREMENT = 1;				//Increment of points per piece, when piece speed is becoming faster
const int SCOREPERPIECE = 10;				//Initial points got by piece stored in board
const int SCOREPERROW = 50;					//Initial points got by deleting a line successfully

class TetrisGame
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	TetrisGame(int screenheight):		
		mScreenHeight(screenheight),
		mBoard(NULL),
		mPieces(NULL),
		mPieceSpeed(INITIALPIECESPEED),
		mLastPieceSpeed(INITIALPIECESPEED),
		mAutoMovements(false),
		mFirstExecution(true),
		mBoardGfxMap(BOARD_WIDTH,BOARD_HEIGHT),
		mNextPieceGfxMap(5,5),
		mScore(0),
		mScoreTicks(0),
		mLines(0),
		mNumPieces(0),
		mPause(false),
		mCurrentPieceSurf(NULL)

	{
		SingletonLogMgr::Instance()->AddNewLine("TetrisGame","Creating board and pieces....",NORMAL);
		
		//Create board and pieces instances
		mPieces = new Pieces;
		mBoard = new Board(mPieces,mScreenHeight);

		//Store position to show next incoming piece
		mNextPosX = 620;
		mNextPosY = 200;

		//Load Resources
		_loadResources();

		// Game initialization
		_initGame ();
		
		SingletonLogMgr::Instance()->AddNewLine("TetrisGame","Game loaded",NORMAL);
	}
	~TetrisGame()
	{
		//Update scores file before exiting
		mScoresFile.AddScore((float)mScore);
		_cleanupMemory();
	}

	//----- GET/SET FUNCTIONS -----
	int GetScore() { return mScore; }
	int GetLines() { return mLines; }
	int GetPieces() { return mNumPieces; }
	int GetNextPosX() { return mNextPosX; }
	int GetNextPosY() { return mNextPosY; }
		//----- OTHER FUNCTIONS -----
	void UpdateGameState(float dt);
	void MovePieceX(bool Positive);
	void MovePieceY();
	void RotateCurrentPiece(bool cw);
	void MovePieceDown();
	void DrawScene();
	void TogglePause(); 
	bool IsGameOver() { return mBoard->IsGameOver();}
	bool IsPaused() { return mPause; }
	void Restart();

private:
	//----- INTERNAL VARIABLES -----
	int mScreenHeight;				// Screen height in pixels
	int mPosX, mPosY;				// Position of the piece that is falling down
	float mPieceSpeed;				// Speed of piece falling down
	float mLastPieceSpeed;			// Memory of updated speed of falling piece (it can be overriden temporaly by a "move down now" command...
	bool mAutoMovements;			// Variable to hold if auto movements are in action, and user cant interact
	bool mFirstExecution;			// Variable to hold if its the first frame of game (update initial variables for game)
	int mScore;						// Point accumulated by user
	int mLines;						// Lines made by user
	int mNumPieces;					// Pieces put in board by user
	bool mPause;					// If game is paused
	unsigned int mScoreTicks;		// How many times bonus points are given to user per piece and row (when piece speed changes)

	HighScores mScoresFile;			// File to store high scores of game

	int mNextPosX, mNextPosY;		// Position of next piece showed in pixels

	Board *mBoard;					// Board class ptr
	Pieces *mPieces;				// Current piece moving
	IND_Timer mMainTimer;			// Internal timer
	IND_Timer mSpeedChangeTimer;		// Internal timer

	//Graphical objects
	IND_Surface mSurfBlockBoard;		//Indielib Images for drawing
	IND_Surface mSurfEmptyBoard;
	IND_Surface mSurfBlockNormal;
	IND_Surface mSurfBlockNormal2;
	IND_Surface mSurfBlockNormal3;
	IND_Surface mSurfBlockNext;
	IND_Surface* mCurrentPieceSurf;
	Generic2DMatrix<GraphicBoardTile*> mBoardGfxMap;
	Generic2DMatrix<GraphicBoardTile*> mNextPieceGfxMap;
	void _storePiece(int posx,int posy);

	//----- INTERNAL FUNCTIONS -----
	void _initGame();
	void _loadResources();
	void _drawPiece (int x, int y);
	void _drawNextPiece ();
	void _drawBoard ();
	void _createNewPiece();
	void _incrementPieceSpeed();
	void _cleanupMemory();
};

#endif
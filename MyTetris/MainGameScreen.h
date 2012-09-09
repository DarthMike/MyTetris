/*
	Filename: MainGameScreen.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Implementation of a screen in game - MAINGAMESCREEN
	Comments: Dependant of IndieLib Graphics library - derived from abtract class "GameScreen"
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _MAINGAMESCREEN
#define _MAINGAMESCREEN

//Library dependencies

//Class dependencies
#include "GameScreen.h"
#include "TetrisGame.h"

const float FADEOUTMESSAGETIME = 1000.0f;		//Time to display messages in screen

class MainGameScreen : public GameScreen
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	MainGameScreen(IndieLibManager* pIlib)
		:GameScreen(pIlib),
		mMainGame(NULL),
		mSoundMessagesSource(-1),
		mExitGame(false)
	{
		_init();	
		SingletonLogMgr::Instance()->AddNewLine("MainGameScreen","MainGameScreen screen loaded",DEBUG);
	}
	virtual ~MainGameScreen()
	{
		_release();
		SingletonLogMgr::Instance()->AddNewLine("MainGameScreen","MainGameScreen screen released",DEBUG);
	}
	//----- GET/SET FUNCTIONS -----
	//----- OTHER FUNCTIONS -----
	//Implementations from base class
	virtual void ExecuteEnter();
	virtual void ExecuteLogicUpdate(float dt);
	virtual void ExecuteRender();
	virtual void ExecuteExit();
	virtual GameState IsExit();
	//----- PUBLIC VARIABLES ------

protected:
	//----- INTERNAL VARIABLES -----
	char mGameOverText[100];
	char mScoreText[100];
	TetrisGame* mMainGame;

	int mSoundMessagesSource;
	bool mExitGame;
	//----- INTERNAL FUNCTIONS -----
	void _init();//Internal init
	void _release(); //Internal release

	void _processGameInputs(IND_Input* inputptr); //Encapsulation of inputs to game
};

#endif
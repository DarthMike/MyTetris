/*
	Filename: GameStateManager.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate logic related to states inside game (credits, intro, maingame...)
	Comments: A Game state is associated to one screen, but many game states can have the same screen. 
	          In this class, the state machine of the main game app is defined and executed. its up to the
			  container screens to tell this manager which state will be next.
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/
#ifndef _GAMESTATEMANAGER
#define _GAMESTATEMANAGER

//Library dependencies

//Class dependencies
#include "General_Resources.h"
#include "TetrisGame.h"
#include "GameScreen.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"
#include "MainGameScreen.h"
#include "CreditsScreen.h"

class GameStateManager
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	GameStateManager():mExit(false),mState(NOSTATE),mPreviousState(NOSTATE)
	{
	}
	~GameStateManager()
	{		
		GameScreen::GameScreenMap::iterator it = mScreensMap.begin();
		while(it != mScreensMap.end())
		{
			if((*it).second)
				delete (*it).second;
			it++;
		}
		mScreensMap.clear();
	}
	//----- GET/SET FUNCTIONS -----

	//----- OTHER FUNCTIONS -----
	void Init();						//Init state
	void Render();						//Render screen contents depending on state
	void UpdateLogic(float dt);			//Update game logic depending on state
	bool Exit() {return mExit;};		//Is game finished?
	//----- PUBLIC VARIABLES ------
protected:
	//----- INTERNAL VARIABLES -----
	GameState mState;						//The game state
	GameState mPreviousState;				//The previous game state
	bool mExit;								//Is necessary to exit?
	GameScreen::GameScreenMap mScreensMap;	//Screens dynamic container
};

#endif
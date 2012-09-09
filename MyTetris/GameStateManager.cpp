/*
	Filename: GameStateManager.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate logic related to states inside game (credits, intro, maingame...)
	Comments: 
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "stdAfx.h"
#include "GameStateManager.h"

//Init state
void GameStateManager::Init()		
{	
	//Load resources for intro sequence
	SingletonIndieLib::Instance()->Render->ClearViewPort(0,0,0);	
	mState = INTRO;  //Initial state
	//Setup state - INTRO
	GameScreen* screen = new SplashScreen(SingletonIndieLib::Instance());
	mScreensMap[INTRO] = screen;
	mScreensMap[INTRO]->ExecuteEnter();
	mPreviousState = mState;
}
//Render screen contents depending on state
void GameStateManager::Render()		
{
	//Only render if a state is fixed
	if(mState == mPreviousState)	
	{
		//Render corresponding screen
		mScreensMap[mState]->ExecuteRender();
	}	
}

//Update game logic depending on state
void GameStateManager::UpdateLogic(float dt)
{	
	//Capture key pressed
	IndieLibManager* pIlib = SingletonIndieLib::Instance();
	IND_Input* pInputptr= pIlib->Input;
	
	//Update Logic
	switch(mState)
	{
	case(INTRO): 
		{
			//-------Enter action of state---------
			//Done in init of class
			//-------Logic for this state----------
			//Execute screen
			mScreensMap[INTRO]->ExecuteLogicUpdate(dt);
			GameState nextstate = mScreensMap[INTRO]->IsExit();
			//-------Change of state:--------------
			if(
				nextstate != NOSTATE
				)
			{
				mScreensMap[mState]->ExecuteExit();
				delete 	mScreensMap[mState];
				mScreensMap.erase(mState);
				mState = nextstate;
				mPreviousState = INTRO;
			}
		}
		break;
	case(MAINMENU):
		{
			//-------Enter action of state---------
			if(mState != mPreviousState)
			{
				//Setup state - MAINMENU
				//Create main menu screen
				GameScreen* screen = new MainMenuScreen(pIlib);
				mScreensMap[MAINMENU] = screen;
				mScreensMap[MAINMENU]->ExecuteEnter();
				mPreviousState = mState;
			}
			//-------Logic for this state----------
			//Execute screen
			mScreensMap[MAINMENU]->ExecuteLogicUpdate(dt);
			GameState nextstate = mScreensMap[MAINMENU]->IsExit();
			//-------Change of state:--------------
			if(
				nextstate != NOSTATE
				)
			{
				//Delete screen
				mScreensMap[mState]->ExecuteExit();
				delete 	mScreensMap[mState];
				mScreensMap.erase(mState);
				mState = nextstate;
				mPreviousState = MAINMENU;
			}
		}
			break;
	case(NORMALGAME):
		{
			//-------Enter action of state---------
			if(mState != mPreviousState)
			{
				//Setup state - NORMAL GAME
				//IF - Screen wasnt created previously
				//Create game screen (and game)
				GameScreen* screen = new MainGameScreen(pIlib);
				mScreensMap[NORMALGAME] = screen;
				mScreensMap[NORMALGAME]->ExecuteEnter();
				mPreviousState = mState;
			}
			//-------Logic for this state----------		
			//Execute screen
			mScreensMap[NORMALGAME]->ExecuteLogicUpdate(dt);

			//Check exit conditions from screen
			GameState nextstate = mScreensMap[NORMALGAME]->IsExit();
			//Change of state: BY SCREEN
			if(
				nextstate != NOSTATE
				)
			{
				mScreensMap[mState]->ExecuteExit();
				//Delete game screen (and game)
				delete 	mScreensMap[mState];
				mScreensMap.erase(mState);
				mState = nextstate;
				mPreviousState = NORMALGAME;
			}
		}
		break;
	case(CREDITS):
		{
			//-------Enter action of state---------
			if(mState != mPreviousState)
			{
				//Setup state - CREDITS
				//Create main menu screen
				CreditsScreen* screen = new CreditsScreen(pIlib);
				mScreensMap[CREDITS] = screen;
				mScreensMap[CREDITS]->ExecuteEnter();
				mPreviousState = mState;
			}
			//-------Logic for this state----------
			//Execute screen
			mScreensMap[CREDITS]->ExecuteLogicUpdate(dt);
			GameState nextstate = mScreensMap[CREDITS]->IsExit();
			//-------Change of state:--------------
			if(
				nextstate != NOSTATE
				)
			{
				//Delete screen
				mScreensMap[mState]->ExecuteExit();
				delete 	mScreensMap[mState];
				mScreensMap.erase(mState);
				mState = nextstate;
				mPreviousState = CREDITS;
			}
		}
		break;
	case(EXIT):
		{
			//Signal application end
			mExit = true;
			SingletonLogMgr::Instance()->AddNewLine("GameStateManager::UpdateLogic","User quits game...",NORMAL);
			//Just wait for app to end
		}
	default:
		break;
	}
}

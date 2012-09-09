/*
	Filename: GameApp.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate Game application functionality (startup and cleanup)
	Comments: 
	Attribution: Based in part of tutorial at gametuto.com - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "stdAfx.h"
#include "GameApp.h"

//System initialization of game
void GameApp::_init()
{
	//Init IndieLib
	IndieLibManager* pIlib = SingletonIndieLib::Instance();
	
	LogManager* pLog = SingletonLogMgr::Instance();
	pLog->AddNewLine("GameApp::_init()","Launching game...",NORMAL);

	if(!pIlib->Init())
		throw(GenericException("Error initialization IndieLib",GenericException::LIBRARY_ERROR));

	//Init GFX Effects
	SingletonGFXEffects::Instance();

	//Init Sound System
	SingletonSoundMgr::Instance();

	//TODO: Init Resources
	//TODO: Init keyboard and mouse mappings from external file

	//Ready to run
	
	//Initialize game state
	mStateManager = new GameStateManager;  //Create a state manager for game (create the game with all states)
	mStateManager->Init();

	//Init timer
	mMainTimer.Start();
	
}

//System release of game
void GameApp::_release()
{
	LogManager* pLog = SingletonLogMgr::Instance();
	pLog->AddNewLine("GameApp::_release()","Game shutdown...",NORMAL);

	//Release Sound System
	SingletonSoundMgr::Instance()->Release();

	//TODO: Release Resources
	pLog->AddNewLine("GameApp::_release()","Freeing resources from game...",NORMAL);

	if(mStateManager)
		delete mStateManager;
	//TODO: Release inputs
	
	//Release IndieLib
	IndieLibManager* pIlib = SingletonIndieLib::Instance();
	pLog->AddNewLine("GameApp::_release()","Release of IndieLib",NORMAL);
	pIlib->End();
}

//Run (Main game Loop)
void GameApp::Run()
{
	//IndieLib pointer
	IndieLibManager* pIlib = SingletonIndieLib::Instance();
	SoundManager* pSmgr = SingletonSoundMgr::Instance();
	
	mMainTimer.Start();

	while(!mStateManager->Exit())
	{		
		//Update time spend calculating 
		mMainTimer.Start();
		//------ Input Capture ------
		//Update input every frame (needed for IndieLib)
		pIlib->Input->Update();
		//------ Render loop ------
		pIlib->Render->BeginScene ();
		mStateManager->Render();
		pIlib->Entity2dManager->RenderEntities2d ();
		//pIlib->Entity2dManager->RenderCollisionAreas (255, 255, 250, 255);  //For debugging purposes
		pIlib->Render->EndScene ();	

		unsigned int dt = mMainTimer.GetTicks();

		//------ Game Logic Update -----
		//Manage states of game - This will update all game logic and things to render
		mStateManager->UpdateLogic((float)dt);
		//Update logic of sound system
		pSmgr->Update((float)dt);
		//Update Graphics Effects
		SingletonGFXEffects::Instance()->Update((float)dt);
	}
}
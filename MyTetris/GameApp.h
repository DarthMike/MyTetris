/*
	Filename: GameApp.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate Game application functionality (startup and cleanup)
	Comments: 
	Attribution: Based in part of tutorial at gametuto.com - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _GAMEAPP
#define _GAMEAPP

//Library dependencies

//Class dependencies
#include "General_Resources.h"
#include "GameStateManager.h"

class GameApp
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	GameApp()
	{
		_init();
	}
	virtual ~GameApp()
	{
		_release();
	}
	//----- GET/SET FUNCTIONS -----
	void Run();
	//----- OTHER FUNCTIONS -----
	//----- PUBLIC VARIABLES ------

protected:
	//----- INTERNAL VARIABLES -----
	GameStateManager* mStateManager;			//Game state manager
	IND_Timer mMainTimer;					    //Main app timer
	//----- INTERNAL FUNCTIONS -----
	void _init();
	void _release();
};

#endif
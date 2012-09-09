/*
	Filename: CreditsScreen.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Implementation of a screen in game - CREDITS
	Comments: Dependant of IndieLib Graphics library - derived from abtract class "GameScreen"
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _CREDITSSCREEN
#define _CREDITSSCREEN

//Library dependencies

//Class dependencies
#include "GameScreen.h"

class CreditsScreen : public GameScreen
{

public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	CreditsScreen(IndieLibManager* pIlib):GameScreen(pIlib)
	{
		_init();	
		SingletonLogMgr::Instance()->AddNewLine("CreditsScreen","Credits screen loaded",DEBUG);
	}
	virtual ~CreditsScreen()
	{
		_release();
		SingletonLogMgr::Instance()->AddNewLine("CreditsScreen","Credits screen released",DEBUG);
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
	//----- INTERNAL FUNCTIONS -----
	void _init();//Internal init
	void _release(); //Internal release
};

#endif
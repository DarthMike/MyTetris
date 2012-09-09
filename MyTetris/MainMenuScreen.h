/*
	Filename: MainMenuScreen.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Implementation of a screen in game - MAINMENUSCREEN
	Comments: Dependant of IndieLib Graphics library - derived from abtract class "GameScreen"
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _MAINMENUSCREEN
#define _MAINMENUSCREEN

//Library dependencies

//Class dependencies
#include "GameScreen.h"
#include "HighScores.h"

class MainMenuScreen : public GameScreen
{
protected:
	typedef enum ScreenSelection{SEL1,SEL2,SEL3,SEL4,NOTHING}ScreenSelection;
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	MainMenuScreen(IndieLibManager* pIlib):GameScreen(pIlib),mSelection(NOTHING)
	{
		_init();	
		SingletonLogMgr::Instance()->AddNewLine("MainMenuScreen","MainMenuScreen screen loaded",DEBUG);
	}
	virtual ~MainMenuScreen()
	{
		_release();
		SingletonLogMgr::Instance()->AddNewLine("MainMenuScreen","MainMenuScreen screen released",DEBUG);
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
	ScreenSelection mSelection;
	char mHighScores[NUMBEROFHIGHSCORES*10];
	//----- INTERNAL FUNCTIONS -----
	void _init();//Internal init
	void _release(); //Internal release
	void _readHighScores();	//Read high scores from file
};

#endif
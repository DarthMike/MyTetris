/*
	Filename: GameScreen.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Abstract class to define a screen of game
	             A game screen only contains graphical data to portray and show. It also will execute the UI
				 code necessary in very game state (or screen in this case), as it can be different for every
				 screen. Pure game logic is done in another classes
	Comments: Dependant of IndieLib Graphics library
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 

	TODO:

	IT IS NECESSARY TO INCLUDE LOADING OF SCREEN LAYOUT FROM XML FILE!!!
	THIS SHOULD BE VIRTUAL HERE AND IMPLEMENTED IN EACH SCREEN
*/
#ifndef _GAMESCREEN
#define _GAMESCREEN

//Library dependencies
#include <map>
#include <string>

//Class dependencies
#include "General_Resources.h"

class GameScreen
{
	//Types definitions
public:
	// A map of screen object with name of screen
	typedef std::map<GameState,GameScreen*> GameScreenMap;
protected:
	typedef std::map<std::string,IND_Entity2d*> EntitiesMap;
	typedef EntitiesMap::iterator EntitiesMapIterator;
	typedef std::map<std::string,IND_Surface*> SurfacesMap;
	typedef SurfacesMap::iterator SurfacesMapIterator;
	typedef std::map<std::string,IND_Font*> FontsMap;
	typedef FontsMap::iterator FontsMapIterator;
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	GameScreen(IndieLibManager* pIlib):mILib(pIlib),mExit(false)
	{
		//Derived class will implement what to load in this member variables
		SingletonLogMgr::Instance()->AddNewLine("GameScreen","New game screen created",NORMAL);
	}
	virtual ~GameScreen()
	{
		bool failure = false;

		//Delete from graphics system resources used
		//and deallocate memory

		//Cleanup Dynamic containers
		EntitiesMapIterator eit;
		for(eit = mEntitiesMap.begin(); eit!=mEntitiesMap.end();eit++)
		{
			if((*eit).second)
			{
				if(!mILib->Entity2dManager->Delete((*eit).second))
					failure = true;
				delete (*eit).second;
			}
		}
		mEntitiesMap.clear();

		SurfacesMapIterator sit;
		for(sit = mSurfacesMap.begin(); sit!=mSurfacesMap.end();sit++)
		{
			if((*sit).second)
			{
				if(!mILib->SurfaceManager->Delete((*sit).second))
					failure = true;
				delete (*sit).second;
			}
		}
		mSurfacesMap.clear();

		FontsMapIterator fit;
		for(fit =mFontsMap.begin(); fit!=mFontsMap.end();fit++)
		{
			if((*fit).second)
			{
				if(!mILib->FontManager->Delete((*fit).second))
					failure = true;
				delete (*fit).second;
			}
		}
		mFontsMap.clear();

		if(failure)
			SingletonLogMgr::Instance()->AddNewLine("GameScreen","Failure releasing resources from screen!",EXCEPTION);

		SingletonLogMgr::Instance()->AddNewLine("GameScreen","Game Screen deleted",NORMAL);

		//Derived class will implement what to delete additionally
	}
	//----- GET/SET FUNCTIONS -----
	//----- OTHER FUNCTIONS -----
	virtual void ExecuteEnter() = 0; //When entering screen
	virtual void ExecuteLogicUpdate(float dt) = 0; //Update screen logic
	virtual void ExecuteRender() = 0; //Render contents of screen (if necesary)
	virtual void ExecuteExit() = 0; //When exiting screen
	virtual GameState IsExit() { return NOSTATE; } //Exit conditions from UI - Exit to another game state
	//----- PUBLIC VARIABLES ------

protected:
	//----- INTERNAL VARIABLES -----
	//Indielib Pointer
	IndieLibManager* mILib;
	//Other entities, surfaces or fonts defined
	EntitiesMap mEntitiesMap;
	SurfacesMap mSurfacesMap;
	FontsMap mFontsMap;

	//Tracking of exit condition
	bool mExit;
	//----- INTERNAL FUNCTIONS -----
};

#endif
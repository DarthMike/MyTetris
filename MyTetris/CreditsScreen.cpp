/*
	Filename: CreditsScreen.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Implementation of a screen in game - CREDITS
	Comments: Dependant of IndieLib Graphics library - derived from abtract class "GameScreen"
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "CreditsScreen.h"

//To execute on start
void CreditsScreen::ExecuteEnter()
{
	mEntitiesMap["BackGround"]->SetShow(true);
	mEntitiesMap["Credits2"]->SetShow(false);
	//Fade screen in
	SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["BackGround"],6,Vector3(0,0,0));
	SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["Credits1"],8,Vector3(0,0,0));

}

//To execute while rendering
void CreditsScreen::ExecuteRender()
{
	//Rendering is done by entities -> IndieLib
}

//Update logic of screen
void CreditsScreen::ExecuteLogicUpdate(float dt)
{
	//IF - Not exit operation
	if(!mExit)
	{
		//Show credits
		//IF - CREDITS 1 COMPLETELY SHOWN CREDITS 2 COMPLETELY DISSAPEARED
		if(
			SingletonGFXEffects::Instance()->IsEffectComplete(mEntitiesMap["Credits1"])
			&&
			mEntitiesMap["Credits1"]->IsShow()
			&&
			!mEntitiesMap["Credits2"]->IsShow())
		{
			//Make credits 1 dissapear
			SingletonGFXEffects::Instance()->FadeOutEntity(mEntitiesMap["Credits1"],2,Vector3(0,0,0));
			//Make credits 2 appear
			SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["Credits2"],10,Vector3(0,0,0));
		}//ELSE IF - CREDITS 1 COMPLETELY DISSAPEARED CREDITS 2 COMPLETELY APPEARED
		else if(
			SingletonGFXEffects::Instance()->IsEffectComplete(mEntitiesMap["Credits2"])
			&&
			!mEntitiesMap["Credits1"]->IsShow()
			&&
			mEntitiesMap["Credits2"]->IsShow())
		{
			//Make credits 2 dissapear
			SingletonGFXEffects::Instance()->FadeOutEntity(mEntitiesMap["Credits2"],2,Vector3(0,0,0));
			//Make credits 1 appear
			SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["Credits1"],10,Vector3(0,0,0));
		}
	}//IF

	//Exit condition
	if(mILib->Input->IsKeyPressed(IND_ESCAPE)) 
	{
		//Fade out everything (with same time)
		SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["BackGround"]);
		SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["Credits1"]);
		SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["Credits2"]);
		SingletonGFXEffects::Instance()->FadeOutEntity(mEntitiesMap["BackGround"],2,Vector3(0,0,0));
		SingletonGFXEffects::Instance()->FadeOutEntity(mEntitiesMap["Credits1"],2,Vector3(0,0,0));
		SingletonGFXEffects::Instance()->FadeOutEntity(mEntitiesMap["Credits2"],2,Vector3(0,0,0));
		mExit = true;
	}
}

//To execute at exit
void CreditsScreen::ExecuteExit()
{
	mEntitiesMap["BackGround"]->SetShow(false);

	//Stop effect if it is in course
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["BackGround"]);
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["Credits1"]);
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["Credits2"]);

}

//Return a game state if it is necessary to exit
GameState CreditsScreen::IsExit()
{
	//Check if it is needed to change screen
	if(
		mExit 
		&&
		SingletonGFXEffects::Instance()->IsEffectComplete(mEntitiesMap["BackGround"])
		)
		return (MAINMENU); //To main menu 
	else
		return(NOSTATE); //No change
}

//Internal init
void CreditsScreen::_init()
{
	//-----------Surfaces adding--------------
	mSurfacesMap["BackGround"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["BackGround"],".\\GameData\\Image\\CreditsBackGround.jpg",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	mSurfacesMap["Credits1"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["Credits1"],".\\GameData\\Image\\CreditsLetters1.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	mSurfacesMap["Credits2"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["Credits2"],".\\GameData\\Image\\CreditsLetters2.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	
	//------------Entities adding--------------
	mEntitiesMap["BackGround"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["BackGround"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));

	mEntitiesMap["Credits1"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["Credits1"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));

	mEntitiesMap["Credits2"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["Credits2"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	
	//Entity->Surface assignment
	mEntitiesMap["BackGround"]->SetSurface(mSurfacesMap["BackGround"]);	
	mEntitiesMap["Credits1"]->SetSurface(mSurfacesMap["Credits1"]);	
	mEntitiesMap["Credits2"]->SetSurface(mSurfacesMap["Credits2"]);	
	
		
	//---------------Fonts adding---------------
	//No fonts
}
//Internal release
void CreditsScreen::_release()
{
	//Release from memory and graphics system is done by base class
}
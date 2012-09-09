/*
	Filename: SplashScreen.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Implementation of a screen in game
	Comments: Dependant of IndieLib Graphics library - derived from abtract class "GameScreen"
	Attribution: 
		License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "SplashScreen.h"

//To execute on start
void SplashScreen::ExecuteEnter()
{
	mEntitiesMap["BackGround"]->SetShow(true);
	mEntitiesMap["TextMessages"]->SetShow(true);
	//Register effects to make
	SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["TextMessages"],3,Vector3(0,-20,0));
}

//To execute while rendering
void SplashScreen::ExecuteRender()
{
	//Rendering is done by entities -> IndieLib
}

//Update logic of screen
void SplashScreen::ExecuteLogicUpdate(float dt)
{
	//Increment time in screen
	mScreenTime +=dt;
	//Display message
	mEntitiesMap["TextMessages"]->SetText("A game by Miguel Angel Quinones");
}

//To execute at exit
void SplashScreen::ExecuteExit()
{
	mEntitiesMap["BackGround"]->SetShow(false);
	mEntitiesMap["TextMessages"]->SetShow(false);
	//Stop effect if it is in course
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["TextMessages"]);
}

//Return a game state if it is necessary to exit
GameState SplashScreen::IsExit()
{
	//Check if it is needed to change screen
	if(
		mScreenTime > SPLASHSCREENTIME   //Time elapsed
		|| 
		mILib->Input->IsKeyPressed(IND_ESCAPE) //User skips screen (ESC)
		)
		return (MAINMENU); //To main menu 
	else
		return(NOSTATE); //No change
}

//Internal init
void SplashScreen::_init()
{
	//-----------Surfaces adding--------------
	mSurfacesMap["BackGround"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["BackGround"],".\\GameData\\Image\\MyTetrisSplash.jpg",IND_OPAQUE,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	
	//------------Entities adding--------------
	mEntitiesMap["BackGround"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["BackGround"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	mEntitiesMap["TextMessages"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["TextMessages"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	
	//Entity->Surface assignment
	mEntitiesMap["BackGround"]->SetSurface(mSurfacesMap["BackGround"]);	
	
		
	//---------------Fonts adding---------------
	mFontsMap["TextMessages"] = new IND_Font;
	if(!mILib->FontManager->Add(mFontsMap["TextMessages"],".\\GameData\\Image\\MainFont_Small.tga", ".\\GameData\\Image\\MainFont_Small.xml", IND_ALPHA, IND_32))
		throw(GenericException("Problem with font loading",GenericException::GRAPHICS_ERROR));
	
	//Layout screen (initial state)
	mEntitiesMap["TextMessages"]->SetFont(mFontsMap["TextMessages"]);
	mEntitiesMap["TextMessages"]->SetHotSpot(0.5f,0.5f);
	mEntitiesMap["TextMessages"]->SetAlign(IND_CENTER);
	mEntitiesMap["TextMessages"]->SetTint(255,0,0);
	mEntitiesMap["TextMessages"]->SetPosition((float)400,(float)500,30);

}
//Internal release
void SplashScreen::_release()
{
	//Release from memory and graphics system is done by base class
}
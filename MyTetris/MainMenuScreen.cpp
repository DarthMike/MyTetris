/*
	Filename: MainMenuScreen.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Implementation of a screen in game - MAINMENUSCREEN
	Comments: Dependant of IndieLib Graphics library - derived from abtract class "GameScreen"
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "MainMenuScreen.h"

//To execute on start
void MainMenuScreen::ExecuteEnter()
{
	mEntitiesMap["BackGround"]->SetShow(true);
	mEntitiesMap["TextMessages"]->SetShow(false);
	mEntitiesMap["ScoreText"]->SetShow(false);
	mEntitiesMap["MenuSelection1"]->SetShow(true);
	mEntitiesMap["MenuSelection2"]->SetShow(true);
	mEntitiesMap["MenuSelection3"]->SetShow(true);
	mEntitiesMap["MenuSelection4"]->SetShow(true);
	mEntitiesMap["SelectionIndicator"]->SetShow(true);
	mEntitiesMap["MouseCursor"]->SetShow(true);

	//Read high scores
	_readHighScores();
	//Effects
	SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["MenuSelection1"],2,Vector3(0,-10,0));
	SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["MenuSelection2"],2,Vector3(10,-5,0));
	SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["MenuSelection3"],2,Vector3(10,5,0));
	SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["MenuSelection4"],2,Vector3(0,10,0));

	//Play background music
	SingletonSoundMgr::Instance()->PlayMusic(std::string("MUSICA"));
}

//To execute while rendering
void MainMenuScreen::ExecuteRender()
{
	//Rendering is done by entities -> IndieLib
}

//Return a game state if it is necessary to exit
GameState MainMenuScreen::IsExit()
{
	//Check if it is needed to change screen
	switch(mSelection)
	{
	case(SEL1):
		{
			return(NORMALGAME);
		}
		break;
	case(SEL2):
		{
			//When in selection 2, no change of state is made!
			return(NOSTATE);
		}
		break;
	case(SEL3):
		{
			return(CREDITS);
		}	
		break;
	case(SEL4):
		{
			return(EXIT);
		}
		break;
	default:
		{
			return(NOSTATE);
		}
		break;
	}
}
//Update logic of screen
void MainMenuScreen::ExecuteLogicUpdate(float dt)
{
	//Update position of mouse cursor
	mEntitiesMap["MouseCursor"]->SetPosition((float)mILib->Input->GetMouseX(),(float)mILib->Input->GetMouseY(),3);

	//Check if mouse was clicked
	bool mousepressed = mILib->Input->IsMouseButtonPressed(IND_MBUTTON_LEFT);

	//SELECTION OF MAIN GAME
	if(mILib->Entity2dManager->IsCollision(mEntitiesMap["MouseCursor"],"body",mEntitiesMap["MenuSelection1"],"body"))
	{
		mEntitiesMap["SelectionIndicator"]->SetPosition(20,mEntitiesMap["MenuSelection1"]->GetPosY(),1);
		if (mousepressed)
		{
			mSelection = SEL1; 
		}
	}
	//SELECTION OF HIGH SCORES
	else if(mILib->Entity2dManager->IsCollision(mEntitiesMap["MouseCursor"],"body",mEntitiesMap["MenuSelection2"],"body"))
	{
		
		mEntitiesMap["SelectionIndicator"]->SetPosition(20,mEntitiesMap["MenuSelection2"]->GetPosY(),1);
		if (mousepressed)
		{
			mSelection = SEL2; 
		}
	}
	//SELECTION TO CREDITS
	else if(mILib->Entity2dManager->IsCollision(mEntitiesMap["MouseCursor"],"body",mEntitiesMap["MenuSelection3"],"body"))
	{
		mEntitiesMap["SelectionIndicator"]->SetPosition(20,mEntitiesMap["MenuSelection3"]->GetPosY(),1);
		if (mousepressed)
		{
			mSelection = SEL3; 
		}
	}
	//SELECTION TO EXIT GAME
	else if(mILib->Entity2dManager->IsCollision(mEntitiesMap["MouseCursor"],"body",mEntitiesMap["MenuSelection4"],"body"))
	{
		mEntitiesMap["SelectionIndicator"]->SetPosition(20,mEntitiesMap["MenuSelection4"]->GetPosY(),1);
		if (mousepressed)
		{
			mSelection = SEL4; 
			mExit = true;  //Exit condition
		}
	}
	else
	{
		mSelection = NOTHING; 
		mExit = false;  //Exit condition
	}

	//Show high scores
	if(mSelection == SEL2)
	{
		if(!mEntitiesMap["TextMessages"]->IsShow())
		{
			//When hovering over this indicator, fade in high scores
			SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["TextMessages"],1.5,Vector3(0,0,0));
			SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["ScoreText"],1.5,Vector3(0,0,0));
		}
	}
	else if(mSelection != NOTHING)
	{
		mEntitiesMap["TextMessages"]->SetShow(false);
		mEntitiesMap["ScoreText"]->SetShow(false);
	}
}

//To execute at exit
void MainMenuScreen::ExecuteExit()
{
	mEntitiesMap["BackGround"]->SetShow(false);
	mEntitiesMap["TextMessages"]->SetShow(false);
	mEntitiesMap["ScoreText"]->SetShow(false);
	mEntitiesMap["MenuSelection1"]->SetShow(false);
	mEntitiesMap["MenuSelection2"]->SetShow(false);
	mEntitiesMap["MenuSelection3"]->SetShow(false);
	mEntitiesMap["MenuSelection4"]->SetShow(false);
	mEntitiesMap["SelectionIndicator"]->SetShow(false);
	mEntitiesMap["MouseCursor"]->SetShow(false);

	//Stop effects
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["MenuSelection1"]);
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["MenuSelection2"]);
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["MenuSelection3"]);
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["MenuSelection4"]);
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["TextMessages"]);
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["ScoreText"]);

	//Stop sounds
	SingletonSoundMgr::Instance()->Stop();
}

//Internal init
void MainMenuScreen::_init()
{
	//-----------Surfaces adding--------------
	//Background
	mSurfacesMap["BackGround"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["BackGround"],".\\GameData\\Image\\MainTitleBackground.jpg",IND_OPAQUE,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	//Menu selection 1
	mSurfacesMap["MenuSelection1"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["MenuSelection1"],".\\GameData\\Image\\MainTitleText1.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	//Menu selection 2
	mSurfacesMap["MenuSelection2"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["MenuSelection2"],".\\GameData\\Image\\MainTitleText2.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	//Menu selection 3
	mSurfacesMap["MenuSelection3"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["MenuSelection3"],".\\GameData\\Image\\MainTitleText3.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	//Menu selection 4
	mSurfacesMap["MenuSelection4"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["MenuSelection4"],".\\GameData\\Image\\MainTitleText4.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	//Selection Indicator
	mSurfacesMap["SelectionIndicator"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["SelectionIndicator"],".\\GameData\\Image\\Piece.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	//Mouse cursor
	mSurfacesMap["MouseCursor"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["MouseCursor"],".\\GameData\\Image\\Cursor.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));

	//------------Entities adding--------------
	//Background
	mEntitiesMap["BackGround"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["BackGround"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Text messages
	mEntitiesMap["TextMessages"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["TextMessages"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//HighScore texts
	mEntitiesMap["ScoreText"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["ScoreText"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Menu selection 1
	mEntitiesMap["MenuSelection1"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["MenuSelection1"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Menu selection 2
	mEntitiesMap["MenuSelection2"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["MenuSelection2"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Menu selection 3
	mEntitiesMap["MenuSelection3"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["MenuSelection3"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Menu selection 3
	mEntitiesMap["MenuSelection4"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["MenuSelection4"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Selection Indicator
	mEntitiesMap["SelectionIndicator"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["SelectionIndicator"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Mouse cursor
	mEntitiesMap["MouseCursor"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["MouseCursor"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));

	//Entity->Surface assignment
	//Background
	mEntitiesMap["BackGround"]->SetSurface(mSurfacesMap["BackGround"]);
	//Menu selections
	mEntitiesMap["MenuSelection1"]->SetSurface(mSurfacesMap["MenuSelection1"]);
	mEntitiesMap["MenuSelection2"]->SetSurface(mSurfacesMap["MenuSelection2"]);
	mEntitiesMap["MenuSelection3"]->SetSurface(mSurfacesMap["MenuSelection3"]);
	mEntitiesMap["MenuSelection4"]->SetSurface(mSurfacesMap["MenuSelection4"]);
	mEntitiesMap["SelectionIndicator"]->SetSurface(mSurfacesMap["SelectionIndicator"]);
	//Mouse cursor
	mEntitiesMap["MouseCursor"]->SetSurface(mSurfacesMap["MouseCursor"]);

	//Entities collisions
	if(!mEntitiesMap["MenuSelection1"]->SetBoundingAreas(".\\GameData\\Image\\MainTitleText1_Collisions.xml"))
		throw(GenericException("Problem with collision file loading (MainTitleText1_Collisions.xml)",GenericException::GRAPHICS_ERROR));	
	if(!mEntitiesMap["MenuSelection2"]->SetBoundingAreas(".\\GameData\\Image\\MainTitleText2_Collisions.xml"))
			throw(GenericException("Problem with collision file loading (MainTitleText2_Collisions.xml)",GenericException::GRAPHICS_ERROR));
	if(!mEntitiesMap["MenuSelection3"]->SetBoundingAreas(".\\GameData\\Image\\MainTitleText3_Collisions.xml"))
			throw(GenericException("Problem with collision file loading (MainTitleText3_Collisions.xml)",GenericException::GRAPHICS_ERROR));
	if(!mEntitiesMap["MenuSelection4"]->SetBoundingAreas(".\\GameData\\Image\\MainTitleText4_Collisions.xml"))
			throw(GenericException("Problem with collision file loading (MainTitleText4_Collisions.xml)",GenericException::GRAPHICS_ERROR));
	if(!mEntitiesMap["MouseCursor"]->SetBoundingAreas(".\\GameData\\Image\\Cursor_Collisions.xml"))
			throw(GenericException("Problem with collision file loading (Cursor_Collisions.xml)",GenericException::GRAPHICS_ERROR));
	
	//---------------Fonts adding---------------
	mFontsMap["SmallFont"] = new IND_Font;
	if(!mILib->FontManager->Add(mFontsMap["SmallFont"],".\\GameData\\Image\\MainFont_Small.tga", ".\\GameData\\Image\\MainFont_Small.xml", IND_ALPHA, IND_32))
		throw(GenericException("Problem with font loading",GenericException::GRAPHICS_ERROR));

	mFontsMap["MediumFont"] = new IND_Font;
	if(!mILib->FontManager->Add(mFontsMap["MediumFont"],".\\GameData\\Image\\MainFont.tga", ".\\GameData\\Image\\MainFont.xml", IND_ALPHA, IND_32))
		throw(GenericException("Problem with font loading",GenericException::GRAPHICS_ERROR));
	
	//Layout screen (initial state)
	//High Scores title
	mEntitiesMap["TextMessages"]->SetFont(mFontsMap["MediumFont"]);
	mEntitiesMap["TextMessages"]->SetHotSpot(0.5f,0.5f);
	mEntitiesMap["TextMessages"]->SetAlign(IND_LEFT);
	mEntitiesMap["TextMessages"]->SetTint(0,255,0);
	mEntitiesMap["TextMessages"]->SetPosition((float)500,(float)200,30);
	mEntitiesMap["TextMessages"]->SetText("HIGH SCORES");

	mEntitiesMap["ScoreText"]->SetFont(mFontsMap["SmallFont"]);
	mEntitiesMap["ScoreText"]->SetHotSpot(0.5f,0.5f);
	mEntitiesMap["ScoreText"]->SetAlign(IND_LEFT);
	mEntitiesMap["ScoreText"]->SetTint(0,255,0);
	mEntitiesMap["ScoreText"]->SetPosition((float)500,(float)230,30);

	//Layout of entities
	mEntitiesMap["MenuSelection1"]->SetPosition(80,260,1);
	mEntitiesMap["MenuSelection2"]->SetPosition(80,320,1);
	mEntitiesMap["MenuSelection3"]->SetPosition(80,380,1);
	mEntitiesMap["MenuSelection4"]->SetPosition(80,440,1);
	mEntitiesMap["SelectionIndicator"]->SetPosition(20,260,1);
	//Mouse cursor
	//Allocated dynamically
}
//Internal release
void MainMenuScreen::_release()
{
	//Release from memory and graphics system is done by base class
}

//Read high scores and make a text out of it
void MainMenuScreen::_readHighScores()
{
	//Open file
	HighScores* scoresfile = new HighScores();  //Creates manager object and opens file
	
	std::string scorestring = scoresfile->GetHighScores();
	//Debug assert
	unsigned int a = scorestring.size();
	assert(scorestring.size() < NUMBEROFHIGHSCORES*20);
	
	//LOOP - Copy to C-string - INDIELIb!!!! :(
	for(unsigned int i = 0; i<scorestring.size();i++)
	{
		//Copy char by char
		mHighScores[i] = scorestring.at(i); 
	}//LOOP END
	
	//Display scores in entity
	mEntitiesMap["ScoreText"]->SetText(mHighScores);
	//Delete object (will close file)
	delete scoresfile;
}
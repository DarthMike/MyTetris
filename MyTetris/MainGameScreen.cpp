/*
	Filename: MainGameScreen.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Implementation of a screen in game - MAINGAMESCREEN
	Comments: Dependant of IndieLib Graphics library - derived from abtract class "GameScreen"
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "MainGameScreen.h"

//To execute on start
void MainGameScreen::ExecuteEnter()
{
	//Layout screen (initial state)
	mEntitiesMap["BackGround"]->SetShow(true);
	mEntitiesMap["TextMessages"]->SetShow(true);
	mEntitiesMap["ScoreText"]->SetShow(true);
	mEntitiesMap["TipText"]->SetShow(true);

	//START message
	mEntitiesMap["TextMessages"]->SetTint(0,200,100);
	mEntitiesMap["TextMessages"]->SetText("START!");
	SingletonGFXEffects::Instance()->ShowTimedEntity(mEntitiesMap["TextMessages"],2.5,Vector3(0,0,0));
	//Play Start sound - without loop
	SingletonSoundMgr::Instance()->PlayRelativeSource(mSoundMessagesSource,"GameStart",false);
	SingletonSoundMgr::Instance()->PlayMusic(std::string("MUSICB"));

	mExitGame = false;
}
void MainGameScreen::ExecuteRender()
{
	//Rendering is done by entities -> IndieLib

	//Call rendering routine from game
	mMainGame->DrawScene();
}

//To execute while rendering
void MainGameScreen::ExecuteLogicUpdate(float dt)
{
	//Process inputs for game
	_processGameInputs(mILib->Input);	
	
	//Process game logic
	mMainGame->UpdateGameState(dt);
	//Update display of score to user
	int userscore = mMainGame->GetScore();
	int pieces = mMainGame->GetPieces();
	int lines = mMainGame->GetLines();
	//(Copy text)
	sprintf(mScoreText,"Score: %d\n\nPieces: %d\n\nLines: %d",userscore,pieces,lines);
	mEntitiesMap["ScoreText"]->SetText(mScoreText);	

	//Game can be paused, ended or running...

	//IF - GAME OVER?
	if(mMainGame->IsGameOver())
	{
		//Play sound when game over - with loop
		SingletonSoundMgr::Instance()->PlayRelativeSource( mSoundMessagesSource, "GameOver1",true); 
		//Display message to "game over"
		if(!mEntitiesMap["TextMessages"]->IsShow())
		{
			mEntitiesMap["TextMessages"]->SetTint(200,0,0);
			mEntitiesMap["TextMessages"]->SetText(mGameOverText);
			SingletonGFXEffects::Instance()->FadeInEntity(mEntitiesMap["TextMessages"],3,Vector3(0,0,0));
		}
		//IF- Maybe user restarts...
		if(mILib->Input->IsKeyPressed(IND_ESCAPE))
		{
			mExit = true; //Quit from window
		}//ELSE- Maybe user restarts...
		else if(mILib->Input->OnKeyPress(IND_RETURN ))
		{
			mExitGame = false;
			mMainGame->Restart();
			//Stop effect on text
			SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["TextMessages"]);
			//Dont play sound of game over
			SingletonSoundMgr::Instance()->StopSource( mSoundMessagesSource);
		}
	}//ELSE - GAME PAUSED? (BUT NOT BECAUSE EXIT PROMPTED)
	else if (mMainGame->IsPaused() && !mExitGame)
	{
		//Stop effect on text
		SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["TextMessages"]);
		mEntitiesMap["TextMessages"]->SetShow(true);
		mEntitiesMap["TextMessages"]->SetText("..Game Paused..");
	}//ELSE - USER PROMPTS TO QUIT GAME?
	else if (!mExitGame && mILib->Input->OnKeyPress(IND_ESCAPE))
	{
		mExitGame = true;			//Memorize user wants to exit
		mMainGame->TogglePause();	//Pause game
		//Stop effect on text
		SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["TextMessages"]);
		mEntitiesMap["TextMessages"]->SetShow(true);
		mEntitiesMap["TextMessages"]->SetTint(0,255,255);
		mEntitiesMap["TextMessages"]->SetText("EXIT GAME?\n\nYes or No?");
	}//ELSE - USER DOESNT WANT TO QUIT GAME
	else  if(mExitGame && mILib->Input->OnKeyPress(IND_N))
	{
		mExitGame = false;			//Reset memory
		mMainGame->TogglePause();	//Unpause game
		mEntitiesMap["TextMessages"]->SetText("");
	}//ELSE - USER WANTS TO QUIT GAME
	else  if(mExitGame && mILib->Input->OnKeyPress(IND_Y))
	{
		mExit = true;   //Exit condition for window -> true
	
	}//ELSE - NOT SPECIAL CASE -> DONT DISPLAY ANYTHING
	else 
	{
		if(!mMainGame->IsPaused() 
			&& 
			SingletonGFXEffects::Instance()->IsEffectComplete(mEntitiesMap["TextMessages"])
			)
		{
			mEntitiesMap["TextMessages"]->SetShow(false);
		}
	}
}

//To execute at exit
void MainGameScreen::ExecuteExit()
{
	mEntitiesMap["BackGround"]->SetShow(false);
	mEntitiesMap["TextMessages"]->SetShow(false);
	mEntitiesMap["ScoreText"]->SetShow(false);
	mEntitiesMap["TipText"]->SetShow(false);
	//Stop Effects
	SingletonGFXEffects::Instance()->StopEffect(mEntitiesMap["TextMessages"]);

	//Stop sounds
	SingletonSoundMgr::Instance()->Stop();
}

//Return a game state if it is necessary to exit
GameState MainGameScreen::IsExit()
{
	//Check if it is needed to change screen
	if(mExit)
		return (MAINMENU); //To main menu 
	else
		return(NOSTATE); //No change
}

//Internal init
void MainGameScreen::_init()
{
	//Create game object
	mMainGame = new TetrisGame(mILib->Window->GetHeight());

	//Register new source to play in sound manager
	mSoundMessagesSource = SingletonSoundMgr::Instance()->CreateSource ();

	//-----------Surfaces adding--------------
	//Background
	mSurfacesMap["BackGround"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["BackGround"],".\\GameData\\Image\\GameBackGround.jpg",IND_OPAQUE,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	//"Next Piece" background
	mSurfacesMap["NextPiece"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["NextPiece"],".\\GameData\\Image\\NextPieceBackGround.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	//Board background
	mSurfacesMap["BoardBackground"] = new IND_Surface;
	if(!mILib->SurfaceManager->Add(mSurfacesMap["BoardBackground"] ,".\\GameData\\Image\\BoardBackground.png",IND_ALPHA,IND_32))
		throw(GenericException("Problem with surface creation",GenericException::GRAPHICS_ERROR));
	
	//------------Entities adding--------------
	//Background
	mEntitiesMap["BackGround"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["BackGround"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Screen Messages font
	mEntitiesMap["TextMessages"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["TextMessages"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//"Next Piece" background
	mEntitiesMap["NextPiece"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["NextPiece"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Board background
	mEntitiesMap["BoardBackground"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["BoardBackground"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Score Text
	mEntitiesMap["ScoreText"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["ScoreText"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Tip Text
	mEntitiesMap["TipText"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["TipText"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	//Next piece text
	mEntitiesMap["NextPieceText"] = new IND_Entity2d;
	if(!mILib->Entity2dManager->Add(mEntitiesMap["NextPieceText"]))
		throw(GenericException("Problem with entity creation",GenericException::GRAPHICS_ERROR));
	
	//Entity->Surface assignment
	//Background
	mEntitiesMap["BackGround"]->SetSurface(mSurfacesMap["BackGround"]);
	//"Next Piece" background
	mEntitiesMap["NextPiece"]->SetSurface(mSurfacesMap["NextPiece"]);
	//Board background
	mEntitiesMap["BoardBackground"]->SetSurface(mSurfacesMap["BoardBackground"]);	
	
	//---------------Fonts adding---------------
	mFontsMap["TextMessages"] = new IND_Font;
	if(!mILib->FontManager->Add(mFontsMap["TextMessages"],".\\GameData\\Image\\MainFont_VeryBig.tga", ".\\GameData\\Image\\MainFont_VeryBig.xml", IND_ALPHA, IND_32))
		throw(GenericException("Problem with font loading",GenericException::GRAPHICS_ERROR));
	mFontsMap["SmallText"] = new IND_Font;
	if(!mILib->FontManager->Add(mFontsMap["SmallText"],".\\GameData\\Image\\MainFont_Small.tga", ".\\GameData\\Image\\MainFont_Small.xml", IND_ALPHA, IND_32))
		throw(GenericException("Problem with font loading",GenericException::GRAPHICS_ERROR));
	mFontsMap["MediumText"] = new IND_Font;
	if(!mILib->FontManager->Add(mFontsMap["MediumText"],".\\GameData\\Image\\MainFont.tga", ".\\GameData\\Image\\MainFont.xml", IND_ALPHA, IND_32))
		throw(GenericException("Problem with font loading",GenericException::GRAPHICS_ERROR));
	
	//Fonts config
	mEntitiesMap["TextMessages"]->SetFont(mFontsMap["TextMessages"]);
	mEntitiesMap["TextMessages"]->SetHotSpot(0.5f,0.5f);
	mEntitiesMap["TextMessages"]->SetAlign(IND_CENTER);
	mEntitiesMap["TextMessages"]->SetLineSpacing(30);
	mEntitiesMap["TextMessages"]->SetFade(200,0,0,255);
	mEntitiesMap["TextMessages"]->SetPosition((float)mILib->Window->GetWidth()/2,(float)mILib->Window->GetHeight()/2,60);

	mEntitiesMap["ScoreText"]->SetFont(mFontsMap["MediumText"]);
	mEntitiesMap["ScoreText"]->SetAlign(IND_LEFT);
	mEntitiesMap["ScoreText"]->SetTint(255,50,50);

	mEntitiesMap["TipText"]->SetFont(mFontsMap["SmallText"]);
	mEntitiesMap["TipText"]->SetAlign(IND_CENTER);
	mEntitiesMap["TipText"]->SetHotSpot(0.0f,0.0f);
	mEntitiesMap["TipText"]->SetTint(255,255,255);

	mEntitiesMap["NextPieceText"]->SetFont(mFontsMap["SmallText"]);
	mEntitiesMap["NextPieceText"]->SetTint(255,100,100);
	mEntitiesMap["NextPieceText"]->SetAlign(IND_LEFT);
	mEntitiesMap["NextPieceText"]->SetText("Next Piece:");

	//Layout screen (initial state)
	//Main Background is ok covering all screen
	//Board background in board position
	mEntitiesMap["BoardBackground"]->SetHotSpot(0.5f,0.5f);
	mEntitiesMap["BoardBackground"]->SetPosition((float)BOARD_POSITION,
												 (float)mILib->Window->GetHeight() - (BLOCK_SIZE*BOARD_HEIGHT/2),
												 1);
	//Next piece background in next piece position
	mEntitiesMap["NextPiece"]->SetPosition((float)mMainGame->GetNextPosX(),
										   (float)mMainGame->GetNextPosY(),
										    1);
	mEntitiesMap["NextPieceText"]->SetPosition((float)mMainGame->GetNextPosX(),
											   (float)mMainGame->GetNextPosY()-20,
										        1);

	//Tips
	mEntitiesMap["TipText"]->SetPosition(400,5,1);
	mEntitiesMap["TipText"]->SetText("Arrow keys move piece,Z to rotate, X to make it fall\nESC to exit game");

	//Score overview
	mEntitiesMap["ScoreText"]->SetPosition(10,40,1);

	//Init variables
	strcpy(mGameOverText,"GAME OVER!\n\nESC: to exit game\n\nENTER: to replay");
}
//Internal release
void MainGameScreen::_release()
{
	//Dynamically allocated memory...
	if(mMainGame)
		delete mMainGame;
	//Release sound source from sound Manager
	SingletonSoundMgr::Instance()->DeleteSource	(mSoundMessagesSource);
	
	//Release from memory and graphics system is done by base class
}

void MainGameScreen::_processGameInputs(IND_Input* inputptr)
{
	//Check input and update state
	if(inputptr->OnKeyPress(IND_KEYRIGHT))
	{
		mMainGame->MovePieceX(true); //Move piece in +
	}
	else if(inputptr->OnKeyPress(IND_KEYLEFT))
	{
		mMainGame->MovePieceX(false); //Move piece in -	
	}
	else if(inputptr->OnKeyPress(IND_KEYDOWN))
	{
		mMainGame->MovePieceY();	 //Move piece in Y one step
	}
	else if(inputptr->OnKeyPress(IND_X))
	{
		mMainGame->MovePieceDown(); 	//Fall down piece directly to ground
	}
	else if(inputptr->OnKeyPress(IND_Z))
	{
		mMainGame->RotateCurrentPiece(true); 		//Clockwise rotation
	}
	else if(inputptr->OnKeyPress(IND_SPACE))  //Toggle pause in game
	{
		//pause game
		mMainGame->TogglePause();
	}
}
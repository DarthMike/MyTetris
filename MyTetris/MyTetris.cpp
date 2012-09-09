/*
	Filename: MyTetris.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Main file to create a game type tetris with a twist...
	Comments: Its my first game! I made a tetris, with a nasty twist... ;)
	Attribution: Initial version based in part of tutorial at gametuto.com - Javier López López (javilop.com),
	             latest version is my own design and build
    License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

//------------------------------INCLUDED LIBRARIES-----------------------------------------------
#include "stdafx.h"  //Precompiled headers for VC++ IDE (LIBRARIES COMMONLY USED ARE STORED THERE)
#include <windows.h> //For windows

//------------------------------INCLUDED CLASSES-------------------------------------------------
#include "General_Resources.h"	//General resources for whole program
#include "GameApp.h"			//The game application class
//------------------------------GLOBAL DEFINITIONS-----------------------------------------------

//------------------------------PROGRAM FUNCTIONS PROTOTYPES-------------------------------------

//***********************************************************************************************

//***********************************************************************************************

//-----------------------------------------------------------------------------------------------
//-------------------------------APPLICATION-----------------------------------------------------
//-----------------------------------------------------------------------------------------------

int main( int argc, char* argv[] )
{
	//THIS IS MY FIRST GAME!!! (Miguel Angel Quinones) Its made using SDL under windows only platform

	//And of course.... the game is a tetris clone, but with a twist... you will discover it if you play

	//+++++++++++++++++++++++++++++ VARIABLE DECLARATIONS+++++++++++++++++++++++++++++++++++++++++++++++++++
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
    //++++++++++++++++++++++++++++++++++++++ CODE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Leak detections
	//Static detections FROM: http://cgempire.com/forum/tutorials-101/easy-memory-leak-detection-c-c-546.html
	//FINDMEMLEAK(-1);

	//Nested in try-catch, when exception... well, show it to user and finish
	try
	{
		//Logging system init
		SingletonLogMgr::Instance()->OpenLogFile();
		//Game!!!
		GameApp MyTetrisGame;
		MyTetrisGame.Run();
	}
	catch(std::exception &e)
	{
		//Show exception message
		MessageBoxA( NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	return 0;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

//-------------------------------APPLICATION END-------------------------------------------------

//-----------------------------------------------------------------------------------------------
//-------------------------------FUNCTIONS-------------------------------------------------------
//-----------------------------------------------------------------------------------------------
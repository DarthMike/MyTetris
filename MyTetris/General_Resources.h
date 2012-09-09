/*
	Filename: General_Resources.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Header file for general resources used by many classes in program 
	Comments: 
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _GENERAL_RESOURCES
#define _GENERAL_RESOURCES

//Templates
#include "Singleton_Template.h"
//Singleton classes
#include "Math.h"
#include "IndieLibManager.h"
#include "GFXEffects.h"
#include "SoundManager.h"
#include "LogManager.h"

//Utility classes
#include "Generic2DMatrix.h"
#include "Vector3.h"

//Exception managing
#include "GenericException.h"

//Game-related definitions									  //CONTROL DEFINITION
typedef enum GameState{INTRO,MAINMENU,NORMALGAME,CREDITS,EXIT,NOSTATE};

#endif
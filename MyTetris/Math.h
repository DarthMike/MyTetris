/*
	Filename: Math.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate math operations in C++ useful
	Comments: 
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _MATH
#define _MATH

//Library dependencies

//Class dependencies
#include "Singleton_Template.h"

class Math
{
public:


public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	Math()
	{
		//Initialize random number generator
		srand ((unsigned int) time(NULL) );
	}
	~Math()
	{}
	//----- GET/SET FUNCTIONS -----
	//----- OTHER FUNCTIONS -----
	int NewRandom(int low, int high);   //Returns a random number between boundaries

};

//Definitions - SINGLETON MATH OBJECT
typedef Singleton<Math> SingletonMath;

#endif
/*
	Filename: Math.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class to encapsulate math operations in C++ useful
	Comments: 
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "StdAfx.h"
#include "Math.h"

//Returns a random number between boundaries
int Math::NewRandom(int low, int high)   
{
	//Modulo operation to transform 0-RAND_MAX to
	//0 - high, then sum of minimum range
	if(high>low)
	{
		return((rand() % (high - low)) + low);
	}
	else
	{	//If not correct parameters a 0 is returned
		return(0);
	}	
}



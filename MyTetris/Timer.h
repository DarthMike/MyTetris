/*
	Filename: Timer.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Timer encapsulation class - Only interface
	Comments: 
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
*/

#ifndef _TIMER
#define _TIMER

//Library dependencies

//Class dependencies
class Timer
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	Timer(): mElapsedTime(0),mLastTicks(0),mPaused(false)
	{}
	virtual ~Timer(void)
	{}
	//----- GET/SET FUNCTIONS -----
	virtual int GetElapsedTime() = 0;
	//----- OTHER FUNCTIONS -----
	virtual void StartCount() = 0;

protected:
	//----- INTERNAL VARIABLES -----
	Uint32 mElapsedTime;
	Uint32 mLastTicks;
	bool mPaused;
	//----- INTERNAL FUNCTIONS -----
};

#endif
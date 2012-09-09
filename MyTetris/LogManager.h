/*
	Filename: LogManager.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Logging class for application
	Comments: 3 modes: Debug, normal, failure; needs of singleton template file
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _LOGMANAGER
#define _LOGMANAGER
//Library dependencies
#include <vector>
#include <fstream>
#include <iostream>
#include <time.h>
//Class dependencies
#include "Singleton_Template.h"

//General definitions
/* 
   DEBUG - ONLY WHEN LOGGING PRIORITY IS DEBUG, IT WILL BE SHOWN
   NORMAL - ALLWAYS SHOWN 
   EXCEPTION - ALLWAYS SHOWN WITH A SPECIAL ICON SHOWING ITS AN ERROR
*/
typedef enum LoggingPriority{DEBUG,EXCEPTION,NORMAL}LoggingPriority;

//DEBUG MODE ONLY WHEN IN DEBUG RELEASE OF VS2008
#ifdef _DEBUG
#define LOGGING DEBUG
#else
#define LOGGING NORMAL
#endif

class LogManager
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	LogManager(): mFileOpened(false), mPriority(LOGGING)
	{
		OpenLogFile(); //Open file at construction
	}
	~LogManager()
	{
		if(mFileOpened)
		{
			_InsertFinalText();
			CloseLogFile(); //Close file at deconstruction
		}
	}
	//----- GET/SET FUNCTIONS -----
	//----- OTHER FUNCTIONS -----
	void AddNewLine (const std::string& functioncall, const std::string& line, LoggingPriority p); //To add a new line to log
	bool OpenLogFile();									//Opens file explicitly (if not open previously)
	bool CloseLogFile();								//Closes file explicitly
	//----- PUBLIC VARIABLES ------	

protected:
	//----- INTERNAL VARIABLES -----
	static const std::string mFileName;					//Filename (constant) of log file
	std::ofstream mFile;							//File stream
	bool mFileOpened;									//Open/close boolean control
	LoggingPriority mPriority;							//Which logs will be really stored
	//----- INTERNAL FUNCTIONS -----
	void _InsertInitialText();												//Initial text
	void _InsertFinalText();												//Final text
	void _AddLine(const std::string& line);									//Internal adding line to log
	void _GetPriorityMark(LoggingPriority p,std::string &text);	//Convert priority to a text mark
};

//Definitions - SINGLETON LOG MANAGER OBJECT
typedef Singleton<LogManager> SingletonLogMgr;

#endif

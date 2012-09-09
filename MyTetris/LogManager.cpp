/*
	Filename: LogManager.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Logging class for application
	Comments: 3 modes: Debug, normal, failure
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/
#include "stdAfx.h"
#include "LogManager.h"

const std::string LogManager::mFileName = "Log.txt";

//To add a new line to log
void LogManager::AddNewLine (const std::string& functioncall, const std::string& line, LoggingPriority p)
{
	//Add lines with optional formatted arguments if priority is to be displayed
	if(p >= mPriority)
	{
		std::string mark;
		_GetPriorityMark(p,mark);
		std::string initialline = "<" + functioncall + ">: ";
		//Write
		_AddLine(mark+initialline+line);
	}
}
//Opens file explicitly (if not open previously)
bool LogManager::OpenLogFile()
{
	//Check first if file was not opened
	if(!mFileOpened)
	{
		//Open file in append mode and only in output mode
		mFile.open(mFileName.c_str(),std::ios::app | std::ios_base::out);

		//******TODO: Manage size of file!!!******
		//File can grow undetermined times in user computer...!

		if(!mFile.fail())
		{
			mFileOpened = true;
			_InsertInitialText();
		}
		else
		{
			mFileOpened = false;
		}
	}		
			
	return mFileOpened;
}

//Closes file explicitly
bool LogManager::CloseLogFile()
{
	//Close unconditionally
	mFile.close();
	if(!mFile.fail())
		mFileOpened = false;
	return !mFileOpened;
}

//To add a new line to log
void LogManager::_AddLine( const std::string& line )
{
	//Check if file was opened
	if(!mFileOpened)
	{
		if(!mFileName.empty())
			OpenLogFile();
	}

	//Write to successfully opened file
	if(mFileOpened)
	{
		mFile <<line.c_str() << std::endl;
	}

}

//Initial text by default
void LogManager::_InsertInitialText()
{
	//Get time and date from computer
	time_t currenttime = time(NULL);
	//Convert to local time	
	char *timetext = ctime(&currenttime);
	
	assert(mFileOpened);
	mFile<<"==================================================================="<<std::endl;
	mFile<<"=======                 LOGGING STARTED                    ========"<<std::endl;
	mFile<<"==================================================================="<<std::endl;
	mFile<<"New log started at: "<<timetext<<std::endl;

}

//Final text by default
void LogManager::_InsertFinalText()
{
	//Get time and date from computer
	time_t currenttime = time(NULL);
	//Convert to local time	
	char *timetext = ctime(&currenttime);
	
	assert(mFileOpened);
	mFile<<"\nEnd of logging at: "<<timetext<<std::endl;
	mFile<<"==================================================================="<<std::endl;
	mFile<<"=======                 LOGGING FINISHED                   ========"<<std::endl;
	mFile<<"==================================================================="<<std::endl;
}

//Convert priority to a text mark
void LogManager::_GetPriorityMark(LoggingPriority p,std::string &text)
{
	switch(p)
	{
	case(DEBUG):
		{
			text = "(D)";
		}
		break;
	case(EXCEPTION):
		{
			text = "(E)";
		}
		break;		
	case(NORMAL):
	default:
		{
			text = "( )";
		}
		break;
	}
}
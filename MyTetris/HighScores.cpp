/*
	Filename: HighScores.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: High scores file and managing
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "stdAfx.h"
#include "HighScores.h"
#include <sstream>

const std::string HighScores::mFileName = ".\\GameData\\HS.txt";

//Store score to file (if it is between 5 higher)
void HighScores::AddScore(float newscore)
{
	//local variables
	int previousscore = 0;

	//LOOP - Search where to store it
	for(int i = 0;i<NUMBEROFHIGHSCORES;i++)
	{
		
		//IF - Is it high score?
		if(
			mHighScores[i] < newscore 
			&&
			newscore != previousscore)
		{
			//LOOP - Update next scores
			for(int j = NUMBEROFHIGHSCORES-1;j>i;j--)
			{
				mHighScores[j] = mHighScores[j-1];
			}//LOOP

			//Update current score and break loop
			mHighScores[i] = static_cast<int>(newscore);
			break;
		}
		//IF - Careful with scores 0!
		if(mHighScores[i] != 0)
			previousscore = mHighScores[i];
	}//LOOP END


}

//Return higher score
std::string HighScores::GetHighScores()	
{
	//Local variables
	std::stringstream scores;

	//LOOP - Through all high scores
	for(int i = 0;i<NUMBEROFHIGHSCORES;i++)
	{
		//IF - There is a score to store
		if(mHighScores[i] != 0)
		{
			scores<<i+1<<".-  "<<mHighScores[i]<<std::endl;
		}//IF
	}//LOOP END
	return(scores.str());
}

//Opens file explicitly (if not open previously)
bool HighScores::_openFile()
{
	bool success = false;;
	//Open file in input / output mode
	mFile.open(mFileName.c_str(), std::ios_base::out | std::ios_base::in);
	//IF - Not fails
	if(!mFile.fail())
	{
		success = true;
		_getScores();
	}
	else //ELSE - Failed, try to create file
	{
		//Close file, clear status and open again only in output mode (if it is not
		//created it will create the file)
		mFile.close();
		mFile.clear();	
		mFile.open(mFileName.c_str(), std::ios_base::out);

		//IF - Failure to open/create file
		if(mFile.fail())
		{
			//LOOP - Reset values of scores
			for(int i = 0;i <NUMBEROFHIGHSCORES;i++)
			{
				mHighScores[i] = 0;
			}//LOOP END

			return false;
		}//IF

		mFile.close();
		mFile.clear();
		//Open file in input / output mode
		mFile.open(mFileName.c_str(), std::ios_base::out | std::ios_base::in);
		if(!mFile.fail())
		{
			success = true;
			_getScores();
		}

	}//IF

	if(!success)
		SingletonLogMgr::Instance()->AddNewLine("HighScores::_openFile","Error opening/creating scores file!",EXCEPTION);

	return success;
}

//Closes file explicitly
bool HighScores::_closeFile()
{
	//Save highScores
	_saveScores();

	//Close , if it is open!
	if(mFile.is_open())
		mFile.close();

	return mFile.fail();
}

//Get HighScores in file
void HighScores::_getScores()
{
	int i = 0;
	//Check if there is something to read it sets eof if there is nothing
	mFile.peek(); 

	//LOOP - SEARCH FOR SCORES
	while(!mFile.eof()&& i < NUMBEROFHIGHSCORES)
	{
		mFile>>mHighScores[i];
		i++;
	}//LOOP END
	
	//WEIRD! - Use Seekg or Seekp to be able to switch
	//between read and write mode!!
	mFile.seekg(std::ios_base::beg);

	//File read and there werent all scores?
	//LOOP - Reset remaining values of scores
	while(i < NUMBEROFHIGHSCORES)
	{
		mHighScores[i] = 0;
		i++;
	}//LOOP END
}

//Save HighScores to file
void HighScores::_saveScores()
{
	if(mFile.is_open())
	{
		//Seek file put pointer to start
		//mFile.seekp(std::ios_base::beg);
		//LOOP - Write values of scores to stream
		for(int i = 0;i <NUMBEROFHIGHSCORES;i++)
		{
			mFile<<mHighScores[i]<<std::endl;
		}//LOOP END
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("HighScores::_saveScores","Error saving scores to file!",EXCEPTION);
	}
}
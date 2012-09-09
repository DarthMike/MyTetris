/*
	Filename: HighScores.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: High scores file and managing
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _HIGHSCORES
#define _HIGHSCORES
//Library dependencies
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

//Class dependencies
#include "LogManager.h"

//Definitions
const int NUMBEROFHIGHSCORES = 5;

class HighScores
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	HighScores()
	{
		_openFile(); //Open file at construction
	}
	~HighScores()
	{
		_closeFile(); //Close file at deconstruction
	}
	//----- GET/SET FUNCTIONS -----
	std::string GetHighScores();   //Return higher scores in std::string object
	//----- OTHER FUNCTIONS -----
	void AddScore(float newscore);   //Store score to file (if it is between 5 higher)
	//----- PUBLIC VARIABLES ------	

protected:
	//----- INTERNAL VARIABLES -----
	static const std::string mFileName;			//Filename (constant) of high scores file
	std::fstream mFile;							//File stream
	int mHighScores[NUMBEROFHIGHSCORES];		//High scores data
	//----- INTERNAL FUNCTIONS -----
	bool _openFile();							//Opens file 
	bool _closeFile();							//Closes file
	void _getScores();						//Get highscores from file
	void _saveScores();					//Save highscores from fil

};

#endif

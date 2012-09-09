/*
	Filename: GraphicBoardTile.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)s
	Description: Graphic object for a board tile - isolates drawing from logic in a Tetris Game
	Comments: 
	Attribution: 
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _GRAPHICBOARDTILE
#define _GRAPHICBOARDTILE

//Library dependencies

//Class dependencies
#include "General_Resources.h"
#include "Board.h"

class GraphicBoardTile
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	//Constructor with only x-y logical position
	GraphicBoardTile(int boardXpos, int boardYpos, int zPos):mPosXPixels(boardXpos),mPosYPixels(boardYpos),mPosZ(zPos),mGfxSurface(NULL)
	{
		//Get handler to graphics library
		IndieLibManager* pIlib = SingletonIndieLib::Instance();
		
		//Add entity to graphic library
		if(!pIlib->Entity2dManager->Add(&mGfxEntity))
			throw(GenericException("Error creating entity for graphics",GenericException::GRAPHICS_ERROR));

		//Reset attributes, as entity will be moved again to origin
		mGfxEntity.SetPosition((float)mPosXPixels,(float)mPosYPixels,0);

		SingletonLogMgr::Instance()->AddNewLine("GraphicBoardTile","New Tile created",DEBUG);
	}
	~GraphicBoardTile()
	{
		//Delete entity from gfx libary
		SingletonIndieLib::Instance()->Entity2dManager->Delete(&mGfxEntity);
		/*if(!SingletonIndieLib::Instance()->Entity2dManager->Delete(&mGfxEntity))
			SingletonLogMgr::Instance()->AddNewLine("GraphicBoardTile","Tile could not be destructed",DEBUG);
		else
			SingletonLogMgr::Instance()->AddNewLine("GraphicBoardTile","Tile destructed",DEBUG);*/
	}
	//----- GET/SET FUNCTIONS -----
	//----- OTHER FUNCTIONS -----
	void SetSurface(IND_Surface* newSurface);
	//----- PUBLIC VARIABLES ------

private:
	//----- INTERNAL VARIABLES -----
	int mPosXPixels;				//Tile position in screen
	int mPosYPixels;
	int mPosZ;						//Z position
	IND_Entity2d mGfxEntity;		//The graphics entity
	IND_Surface* mGfxSurface;		//The graphics surface assigned to entity
	//----- INTERNAL FUNCTIONS -----
};

#endif
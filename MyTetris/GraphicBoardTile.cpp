/*
	Filename: GraphicBoardTile.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Graphic object for a board tile - isolates drawing from logic in a Tetris Game
	Comments: 
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "stdAfx.h"
#include "GraphicBoardTile.h"

//Set surface to display
void GraphicBoardTile::SetSurface(IND_Surface* newSurface)
{

	//OPTIMIZATION: Only reassign if surface changed
	if(!mGfxSurface 
		||
		newSurface != mGfxSurface 
		)
	{
		mGfxSurface = newSurface;
		
		//Set surface of entity
		mGfxEntity.SetSurface(mGfxSurface);

		//Reset attributes, as entity will be moved again to origin
		mGfxEntity.SetPosition((float)mPosXPixels,(float)mPosYPixels,mPosZ);
	}

}
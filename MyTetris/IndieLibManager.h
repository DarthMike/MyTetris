/*
	Filename: IndielibManager.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Manager class to wrap INDIELIB library
	Comments: 
	Attribution: IndieLib creator of course! - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/
#ifndef _INDIELIBWRAPPER
#define _INDIELIBWRAPPER

//Library dependencies
#include "Indie.h"

//Class dependencies
#include "Singleton_Template.h"

class IndieLibManager
{
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	IndieLibManager():mSystemInitialized(false)
	{}
	~IndieLibManager()
	{
	}
	//----- GET/SET FUNCTIONS -----
	//----- OTHER FUNCTIONS -----
	bool Init ();
	void End ();
	//----- PUBLIC VARIABLES ------
	// ----- IndieLib objects -----
	IND_3dMeshManager		*MeshManager;
	IND_Input				*Input;
	IND_Window				*Window;
	IND_Render				*Render;
	IND_LightManager		*LightManager;
	IND_ImageManager		*ImageManager; 
	IND_SurfaceManager		*SurfaceManager;
	IND_AnimationManager	*AnimationManager;
	IND_FontManager			*FontManager;
	IND_Entity2dManager		*Entity2dManager;
	IND_Entity3dManager		*Entity3dManager;
	IND_Math				*Math;

private:
	//----- INTERNAL VARIABLES -----
	bool mSystemInitialized;		//System init tracking
	//----- INTERNAL FUNCTIONS -----
	void ResetCurrentDirectory_W( void );
};

//SINGLETON INDIELIB MANAGER
typedef Singleton<IndieLibManager> SingletonIndieLib;

#endif
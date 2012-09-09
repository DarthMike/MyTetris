/*
	Filename: IndielibManager.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Manager class to wrap INDIELIB library
	Comments: 
	Attribution: IndieLib creator of course! - Javier López López (javilop.com)
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "stdAfx.h"
#include "IndieLibManager.h"

/* === Windows === */
#include "TCHAR.h"

/*
==================
Sets working path to the directory of exe file (Windows)
==================
*/
void IndieLibManager::ResetCurrentDirectory_W( void )
{
	TCHAR app_path[MAX_PATH] = _T("");
	DWORD size_in_tchars = sizeof(app_path)/sizeof(TCHAR);
	// get full app path with the exe filename
	GetModuleFileName( 0, app_path, size_in_tchars-1 );
	// making app_path to end on dir char ('\\')
	// _tcsrchr - search for char from the string's end
	TCHAR* app_dir = _tcsrchr(app_path, _T('\\'));
	if (app_dir) {
		app_dir += 1;
		if (app_dir) { 
			*app_dir = 0; 
			SetCurrentDirectory(app_path);
			return;
		}
	}
	// TODO. inform somehow that func is failed.
}


/*
==================
Init IndieLib
==================
*/
bool IndieLibManager::Init()
{
	//Init protection 
	if(!mSystemInitialized)
	{
		ResetCurrentDirectory_W();

		// IndieLib Initialization, a debug.log file will be created.
		IndieLib::Init (IND_DEBUG_MODE);
			
		Input				=   new		IND_Input;
		Window				=	new		IND_Window;
		Render				=	new		IND_Render;
		LightManager		=	new		IND_LightManager;
		ImageManager		=	new		IND_ImageManager;
		SurfaceManager		=	new		IND_SurfaceManager;
		MeshManager			=	new		IND_3dMeshManager;
		AnimationManager	=	new		IND_AnimationManager;
		FontManager			=	new		IND_FontManager;
		Entity2dManager		=	new		IND_Entity2dManager;
		Entity3dManager		=	new		IND_Entity3dManager;
		Math				=	new		IND_Math;

		if (!Window				->Init ("", 800, 600, 32, 0, 0))					return 0;
		if (!Render				->Init (Window))									return 0;
		if (!LightManager		->Init (Render))									return 0;
		if (!ImageManager		->Init ())											return 0;
		if (!SurfaceManager		->Init (ImageManager, Render))						return 0;
		if (!AnimationManager	->Init (ImageManager, SurfaceManager))				return 0;
		if (!FontManager		->Init (ImageManager, SurfaceManager))				return 0;	
		if (!Entity2dManager	->Init (Render))									return 0;
		if (!Entity3dManager	->Init (Render))									return 0;
		if (!MeshManager		->Init (Render))									return 0;
		if (!Input				->Init (Render))									return 0;
		if (!Math				->Init ())											return 0;

		mSystemInitialized = true;
	}

	return 1;
}


/*
==================
Free Indielib managers
==================
*/
void IndieLibManager::End()
{
	// ----- Freeing objects -----
	
	Math				->End();
	MeshManager			->End();
	Input				->End();
	Entity2dManager		->End();
	Entity3dManager		->End();
	FontManager			->End();
	AnimationManager	->End();
	SurfaceManager		->End();
	ImageManager		->End();
	LightManager		->End();
	Render				->End();
	Window				->End();

	IndieLib::End ();

	mSystemInitialized = false;
}


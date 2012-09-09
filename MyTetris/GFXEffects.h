/*
	Filename: GFXEffects.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Graphics effects encapsulation
	Comments: It is used with IndieLib
	EFFECTS ADDED:
		FADEIN: Display for a time the entity incrementing its alpha, so it appears slowly to screen
		FADEOUT: Display for a time the entity, maybe moving it and decrementing its alpha, so it dissapears
		TIMED: Display for a time this entity.
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _GFXEffects
#define _GFXEffects

//TODO: IMPLEMENT LOTS OF COOL EFFECTS! ;)

//Library dependencies	

//Class dependencies
#include "LogManager.h"
#include "IndieLibManager.h"
#include "Vector3.h"

class GFXEffects
{
	//Definitions
public:
	typedef enum EffectType{FADEIN,FADEOUT,SHOWTIMED}; //Effects types
	
	typedef struct EffectsParams{
		EffectsParams()
			:effect(FADEIN),
			remainingtime(0.0f),
			timelimit(5.0f),
			param(0),
			movement(0,0,0),
			entity(NULL)
		{}
		EffectType effect;
		float remainingtime;
		float timelimit;
		float param;		//IMPORTANT: HERE WE STORE TEMPORAL VALUES, AS FOR GRANULARITY, SOMETIMES
						    //WORKING WITH TYPES LIKE UNSIGNED BYTE WONT WORK EVERY UPDATE (DT IS SMALL)
		Vector3 movement;
		IND_Entity2d* entity;
	}EffectsParams; //Struct where we hold info about effect

protected:
	typedef std::list<EffectsParams*> EffectsEntitiesList;
	typedef EffectsEntitiesList::iterator EffectsEntitiesListIterator;
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	GFXEffects()
	{
		//Store a copy to graphics manager once constructed
		mGFXLibraryMgr = SingletonIndieLib::Instance();
	}
	~GFXEffects()
	{
		if(!mAffectedEntities.empty())
		{
			SingletonLogMgr::Instance()->AddNewLine("GFXEffects","Graphics effects destructed but there are still effects in course!",EXCEPTION);
			_cleanUp();
			mAffectedEntities.clear();
		}
		else
			SingletonLogMgr::Instance()->AddNewLine("GFXEffects","Graphics Effects manager released",NORMAL);
	}
	//----- GET/SET FUNCTIONS -----
	//----- OTHER FUNCTIONS -----
	void FadeInEntity(IND_Entity2d* entity,float fadetime, Vector3 &MovSpeed);   //Mark entity to make effect - FADEIN
	void FadeOutEntity(IND_Entity2d* entity,float fadetime, Vector3 &MovSpeed);  //Mark entity to make effect - FADEOUT
	void ShowTimedEntity(IND_Entity2d* entity, float time, Vector3 &MovSpeed);   //Mark entity to make effect - TIMEDSHOW
	void Update(float dt);						//Update effects display
	void StopEffect(IND_Entity2d* entity);	//Stop effects of an entity (And unregister it as affected)
	bool IsEffectComplete(IND_Entity2d* entity);   //Prompts if effects are complete for an entity
	void ClearScreen();							    //Clear screen to black

protected:
	//----- INTERNAL VARIABLES -----
	IndieLibManager* mGFXLibraryMgr;
	EffectsEntitiesList mAffectedEntities;		 //A vector storing which entities (text entities)will have effects
	//----- INTERNAL FUNCTIONS -----
	EffectsEntitiesListIterator _getAffectedEntity(IND_Entity2d* entity);  //Gets an entity from the list (if exists)
	bool _foundEffect(IND_Entity2d *entity,EffectType type);
	void _updateFadeIn(float dt, EffectsParams* affected);
	void _updateFadeOut(float dt, EffectsParams* affected);
	void _updateShowTimed(float dt, EffectsParams* affected);
	void _cleanUp();

};

//Singleton GFX object
typedef Singleton<GFXEffects> SingletonGFXEffects;

#endif
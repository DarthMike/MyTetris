/*
	Filename: SoundManager.h
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class which encapsulates managing of sound system
	Comments: 
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#ifndef _SOUNDMANAGER
#define _SOUNDMANAGER

//Library dependencies
#include <string>
#include <map>
#include <queue>
#include <vector>
#include "al.h"   //OPENAL

//Classes dependencies
#include "LogManager.h" 
#include "GenericException.h"
#include "SoundDefs.h"
#include "OggStream.h"

//Definitions
const float SENSCAMBIOVOL = 1.3f;    //When change of volume, sensibility for each change
const float SENSFADEOUT = 0.06f;	 //The same as above but for fadeout
const float GAINLIMIT = 1.0f;		 //GAIN limit for source playing
//const int STREAMBUFFERSIZE = 32768;  //(4096*8) Streaming buffers size

class SoundManager
{
	//Definitions
public:
	typedef struct InfoSource
	{
		InfoSource():source(-1),reserved(false),actionplayed("")
		{}
		ALuint	source;				//ID of source
		bool	reserved;			//Is it reserved
		std::string actionplayed;	//Action assigned
	}InfoSource;  //Source info
	
	typedef std::string	Action;	//Name of action to play
	typedef ALuint	IdBuffer;	//BufferID for an action

	typedef struct FadeOutInfo		
	{
		FadeOutInfo(): used(false), source(-1) {}
		int		source;
		bool	used;
	}FadeOutInfo;		//Info for sources reserved to make a "fadeout"

public:		
	//----- CONSTRUCTORS/DESTRUCTORS -----
	SoundManager(): mSoundON(true), mPause(false),mMusic(NULL)
	{
		//Init system
		mSoundON = _initAL();

		//Load default sounds
		LoadSounds();
	}
	~SoundManager()
	{
		//Release resources if not made previously
		Release();
	}
	//----- GET/SET FUNCTIONS -----
	void	SetGain( float gain );
	float	GetGain();
	std::string GetALErrorString(ALenum err);
	//SOURCE DATA
	bool	SetSourceGain		( int source, float inGain );
	bool	GetSourceGain		( int source, float& outGain );
	bool	SetSourcePosition	( int source, float posx, float posy, float posz);
	bool	GetSourcePosition	( int source, float& posx, float& posy, float& posz);
	bool	SetSourceSpeed	( int source, float velx, float vely, float velz);
	bool	GetSourceSpeed	( int source, float& velx, float& vely, float& velz);

	//LISTENER DATA
	void	SetListenerPosition		( float posx, float posy, float posz);
	void	GetListenerPosition		( float& posx, float& posy, float& posz);
	void	SetListenerSpeed		( float velx, float vely, float velz);
	void	GetListenerSpeed		( float& velx, float& vely, float& velz);
	void	SetListenerOrientation	(	float lookatx, float lookaty, float lookatz,
											float upx, float upy, float upz );
	void	GetListenerOrientation	(	float& lookatx, float& lookaty, float& lookatz,
											float& upx, float& upy, float& upz );
		
	//----- OTHER FUNCTIONS -----
	bool	LoadSound( const std::string& file, const std::string & actionName );
	void	LoadSounds();
	void	Reset();
	void	Pause();
	void	Stop();
	void	SoundOn() {mSoundON=true;}
	void	SoundOff() {mSoundON=false;}
	void ToTurnUp(float amount);
	void ToTurnDown(float amount);     
	bool FadeOut ( int source );
	void Update(float dt);
	void Release();

	//ACTION FUNCTIONS
	bool	PlayAction	( const std::string& action);  //Without position - 2D - ACTION IS A SOUND WHICH WILL NOT BE MODIFIED WHILE PLAYED
	bool	PlayAction	( const std::string& action, float posx, float posy, float posz); //Without position - 3D - ACTION IS A SOUND WHICH WILL NOT BE MODIFIED WHILE PLAYED

	//MUSIC FUNCTIONS
	bool PlayMusic (std::string &ActiontoPlay);		//Play music
	void StopMusic ();							//Stop music
		
	//SOURCE FUNCTIONS
	int		CreateSource		( void );
	bool	DeleteSource		( int source );
    bool	PlaySource			( int source, const std::string& action, bool loop ); 
	bool	PlayRelativeSource	( int source, const std::string& action, bool loop );
	bool	PauseSource			( int source );
	bool	StopSource			( int source );

	//----- PUBLIC VARIABLES -----
	
protected:
	//----- INTERNAL VARIABLES -----
	bool	mSoundON;
	bool	mPause;
	std::map<Action, IdBuffer>		mBuffers;
	std::map<Action, std::string>	mMusicActionMap;
	std::vector<InfoSource>			mSources;
	
	OggStream* mMusic;		//Music Stream (Ogg file)

	//----- INTERNAL FUNCTIONS -----
	FadeOutInfo	mFadeOutSources[3];
	bool _initAL();
	void _releaseAL();
	bool _loadSound	(const std::string& file, IdBuffer& buffer );
	void _clear();
	int	_getSource(bool reserved=false );
};

//Singleton typedef
typedef Singleton<SoundManager> SingletonSoundMgr;

#endif

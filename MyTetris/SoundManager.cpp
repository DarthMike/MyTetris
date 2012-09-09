/*
	Filename: SoundManager.cpp
	Author: Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Description: Class which encapsulates managing of sound system
	Comments: 
	Attribution:
	License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
	         Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

#include "stdAfx.h"
#include "alc.h"
#include "alut.h"
#include "SoundManager.h"

//Loads a sound directly from file
bool SoundManager::LoadSound( const std::string& file, const std::string & actionName )
{
	//Buscamos si tenemos registrada la acción "action"
	std::map<Action, IdBuffer>::iterator it = mBuffers.find( actionName );
	if( it == mBuffers.end() )
	{
		IdBuffer idBuffer;
		if( _loadSound(file, idBuffer) )
		{
			mBuffers.insert(std::pair<Action, IdBuffer>(actionName, idBuffer) );
			return true;
		}
	}
	return false;
}

//Loads sounds
//TODO: IMPLEMENT AUDIO READING FROM XML
void SoundManager::LoadSounds()
{
	bool failure = false;

	//Sounds
	if(!LoadSound(".\\GameData\\Audio\\2341__Jovica__STAB_016_mastered_16_bit.wav", "PieceFall1"))
		failure = true;

	if(!LoadSound(".\\GameData\\Audio\\13401__schluppipuppie__wasserflasche37.wav", "PieceFall2"))
		failure = true;

	if(!LoadSound(".\\GameData\\Audio\\17200__Bitbeast__Wibble.wav", "LineErased1"))
		failure = true;

	if(!LoadSound(".\\GameData\\Audio\\11221__jnr_hacksaw__Zap.wav", "LineErased2"))
		failure = true;

	if(!LoadSound(".\\GameData\\Audio\\15294__Hell_s_Sound_Guy__SMALL_GROUP_LAUGHTER.wav", "GameOver1"))
		failure = true;

	if(!LoadSound(".\\GameData\\Audio\\16832__Prokur__Govorit_i_pokazivaet_Moskva.wav","GameStart"))
		failure = true;

	//Music
	//Define music files  - music files are loaded when needed and played in "stream" mode
	mMusicActionMap["MUSICA"] = ".\\GameData\\Audio\\ManuelMora-TetrisArcade.ogg";
	mMusicActionMap["MUSICB"] = ".\\GameData\\Audio\\tetris.ogg";

	if(failure)
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::LoadSounds","Error loading sound files",EXCEPTION);
	}
}

//Sets global gain for sound
void SoundManager::SetGain(float gain)
{
	alListenerf(AL_GAIN,gain);
}

//Gets global gain for sound
float SoundManager::GetGain()
{
	float gain;
	alGetListenerf(AL_GAIN,&gain);
	return gain;
}

//Increments general volume
void SoundManager::ToTurnUp(float amount)
{
	//Increment volume in "LISTENER"
	
	//Local variables
	float CurrentGain;

	//Value to increment
	CurrentGain = GetGain();
    //Increment
	CurrentGain += amount;
	//Limits
	if (CurrentGain > GAINLIMIT)
	{
		CurrentGain = GAINLIMIT;
	}
	//Set gain incremented
	SetGain(CurrentGain);
}

//Decrements general volume
void SoundManager::ToTurnDown(float amount)
{
    //Reduce volume in "LISTENER"
	
	//Local variables
	float CurrentGain;

	//Value to decrement
	CurrentGain = GetGain();
	//Decrement
	CurrentGain -= amount;
	//Limits
	if (CurrentGain < 0)
	{
		CurrentGain = 0;
	}
	//Set gain decremented
	SetGain(CurrentGain);
}

//Reset system
void SoundManager::Reset()
{
	Stop();
	_clear();
}

//Pause sound playing
void SoundManager::Pause()
{
	ALint state;
	std::vector<InfoSource>::iterator it;
	mPause = !mPause;

	for( it = mSources.begin(); it != mSources.end(); ++it )
	{
		alGetSourcei( (*it).source, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING && mPause)
		{
			alSourcePause((*it).source);
		}
		else if (state == AL_PAUSED && !mPause)
		{
			alSourcePlay((*it).source);
		}
	}
}

//Stop sounds playing
void SoundManager::Stop()
{
	ALint state;
	std::vector<InfoSource>::iterator it;

	for( it = mSources.begin(); it != mSources.end(); ++it )
	{
		alGetSourcei((*it).source,AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING || state == AL_PAUSED)
		{
			alSourceStop((*it).source);
		}
	}

	if(mMusic)
	{
		mMusic->Stop();
	}
}

void SoundManager::Update ( float dt )
{
	//Makes periodic sound updates if necessary
	//Update stream playing (if exists)
	if(mMusic)
		mMusic->Update();
	//Local variables
	int i;
	float SourceGain;

	//--------Sounds fadeout in "Source"-----------------
	for(i=0;i<3;i++)
	{
		if (mFadeOutSources[i].used)
		{
			//Labeled position to make "FadeOut"
			//Decrement value of gain
			GetSourceGain(mFadeOutSources[i].source,SourceGain);
			SourceGain -= dt * SENSFADEOUT;
			SetSourceGain(mFadeOutSources[i].source,SourceGain);
		}
	}
	//-------------------------------------------------------

}

//Perform cleanup
void SoundManager::Release()
{
	//Dynamic memory allocation
	if(mMusic)
	{
		delete mMusic;
		mMusic = NULL;
	}

	if(mSoundON)
	{
		//Release
	  	_releaseAL();
		mSoundON = false;
	}
}

//Play a sound - 3D sound (action) - AN ACTION IS NOT MODIFIED DURING PLAYING
bool SoundManager::PlayAction( const std::string& action, float posx, float posy, float posz)
{
	if (mSoundON)
	{	
		std::map<Action, IdBuffer>::iterator it = mBuffers.find(action);
		if (it==mBuffers.end())
		{
			SingletonLogMgr::Instance()->AddNewLine("SoundManager::PlayAction","Action " + action + " not found",EXCEPTION);
			return false;
		}
		IdBuffer idBuffer = (*it).second;
		int index;
		ALfloat Pos[3] = { posx, posy, posz }; //3d pos
		ALfloat Vel[3] = { 0.0, 0.0, 0.0 };
		
		index = _getSource(false);
	
		if (index==-1)
			return false;
	
		alSourcei (mSources[index].source, AL_SOURCE_RELATIVE, AL_FALSE);
		alSourcei (mSources[index].source, AL_BUFFER, idBuffer);
		alSourcef (mSources[index].source, AL_PITCH, 1.0);
		alSourcef (mSources[index].source, AL_GAIN, 1.0);
		alSourcefv(mSources[index].source, AL_POSITION, Pos);
		alSourcefv(mSources[index].source, AL_VELOCITY, Vel);
		alSourcei (mSources[index].source, AL_LOOPING, AL_FALSE);
	
		alSourcePlay(mSources[index].source);
	}
	return true;
}

//Play a sound - 2D sound (action) - AN ACTION IS NOT MODIFIED DURING PLAYING
bool SoundManager::PlayAction(const std::string& action)
{
	if (mSoundON)
	{	
		//Check if there is an action registered
		std::map<Action, IdBuffer>::iterator it = mBuffers.find(action);
		if (it==mBuffers.end())
		{
			SingletonLogMgr::Instance()->AddNewLine("SoundManager::PlayAction","Action " + action + " not found",EXCEPTION);
			return false;
		}
		IdBuffer idBuffer = (*it).second;
		int index;
		ALfloat vector[3] = { 0.0, 0.0, 0.0 }; //2D SOUND
			
		index = _getSource(false);
	
		if (index==-1)
			return false;

		alSourcei (mSources[index].source, AL_BUFFER, idBuffer);
		alSourcei (mSources[index].source, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcef (mSources[index].source, AL_PITCH, 1.0);
		alSourcef (mSources[index].source, AL_GAIN, 1.0);
		alSourcefv(mSources[index].source, AL_POSITION, vector);
		alSourcefv(mSources[index].source, AL_VELOCITY, vector);
		alSourcefv(mSources[index].source, AL_DIRECTION, vector);
		alSourcei (mSources[index].source, AL_LOOPING, AL_FALSE);
	
		alSourcePlay(mSources[index].source);
	}
	return true;
}

//Create a source
int SoundManager::CreateSource()
{
	return _getSource(true);
}

//Delete a source
bool SoundManager::DeleteSource(int source)
{
	if (source >= 0 && (unsigned int)source<mSources.size())
	{
		StopSource(source);
		mSources[source].reserved = false;
	}
	return true;
}

//Play a source relative (2d)
bool SoundManager::PlayRelativeSource(int source, const std::string& action, bool loop)
{
	if (mSoundON)
	{
		if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
		{
			ALboolean alloop;
	
			//Finf action to play
			if (mBuffers.find(action)==mBuffers.end())
			{
				SingletonLogMgr::Instance()->AddNewLine("SoundManager::PlayRelativeSource","Action " + action + " not found",EXCEPTION);
				return false;
			}

			//If it has to loop, and it is playing, dont do it again
			if (loop)
			{
				//Get state of source
				ALenum state;
				alGetSourcei(mSources[source].source, AL_SOURCE_STATE, &state);

				//Playing?
				if(state == AL_PLAYING && mSources[source].actionplayed == action)
					return true;  //Just return, playing with loop
			
				StopSource(source);
				alloop = AL_TRUE;

			}
			else
			{
				StopSource(source);
				alloop = AL_FALSE;
			}
			//Memorize what is it playing
			mSources[source].actionplayed = action;
				
			ALfloat vector[3] = { 0.0, 0.0, 0.0 };
			alSourcei (mSources[source].source, AL_BUFFER, mBuffers[action]);
			alSourcei (mSources[source].source, AL_SOURCE_RELATIVE, AL_TRUE);
			alSourcef (mSources[source].source, AL_PITCH, 1.0);
			alSourcef (mSources[source].source, AL_GAIN, 1.0);
			alSourcefv(mSources[source].source, AL_POSITION, vector);
			alSourcefv(mSources[source].source, AL_VELOCITY, vector);
			alSourcefv(mSources[source].source, AL_DIRECTION, vector);
			alSourcei (mSources[source].source, AL_LOOPING, alloop);
			alSourcePlay(mSources[source].source);
	
			return true;
		}	
		else
		{
			SingletonLogMgr::Instance()->AddNewLine("SoundManager::PlayRelativeSource","Can't play source. Invalid source " + source,EXCEPTION);
			return false;
		}
	}
	return true;
}

//Play a source 3D
bool SoundManager::PlaySource(int source, const std::string& action, bool loop)
{
	if (mSoundON)
	{
		if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
		{

			if (mBuffers.find(action)==mBuffers.end())
			{
				SingletonLogMgr::Instance()->AddNewLine("SoundManager::PlaySource","Action " + action + " not found",EXCEPTION);
				return false;
			}
	
			ALboolean alloop;
			//If it has to loop, and it is playing, dont do it again
			if (loop)
			{
				//Get state of source
				ALenum state;
				alGetSourcei(mSources[source].source, AL_SOURCE_STATE, &state);

				//Playing?
				if(state == AL_PLAYING && mSources[source].actionplayed == action)
					return true;  //Just return, playing with loop
			
				StopSource(source);
				alloop = AL_TRUE;

			}
			else
			{
				StopSource(source);
				alloop = AL_FALSE;
			}

			//Memorize what is it playing
			mSources[source].actionplayed = action;

			alSourcei (mSources[source].source, AL_SOURCE_RELATIVE, AL_FALSE);
			alSourcei (mSources[source].source, AL_BUFFER, mBuffers[action]);
			alSourcef (mSources[source].source, AL_PITCH, 1.0);
			alSourcef (mSources[source].source, AL_GAIN, 1.0);
			alSourcei (mSources[source].source, AL_LOOPING, alloop);
	
			alSourcePlay(mSources[source].source);
	
			return true;
		}	
		else
		{
			SingletonLogMgr::Instance()->AddNewLine("SoundManager::PlaySource","Can't play source. Invalid source " + source,EXCEPTION);
			return false;
		}
	}
	return true;
}

//Pause a source
bool SoundManager::PauseSource(int source)
{
	if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
	{
		ALint state;
		
		alGetSourcei(mSources[source].source,AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
		{
			alSourcePause(mSources[source].source);
		}
		else if (state == AL_PAUSED)
		{
			alSourcePlay(mSources[source].source);
		}		
		return true;
	}
	else
	{
			SingletonLogMgr::Instance()->AddNewLine("SoundManager::PauseSource(","Can't pause source. Invalid source " + source,EXCEPTION);
		return false;
	}
}

//Stop playing a source
bool SoundManager::StopSource(int source)
{
	if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
	{
		ALint state;
		
		alGetSourcei(mSources[source].source,AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING || state== AL_PAUSED)
		{
			alSourceStop(mSources[source].source);
		}
		return true;
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::StopSource(","Can't stop source. Invalid source " + source,EXCEPTION);
		return false;
	}
	
}

bool SoundManager::FadeOut ( int source )
{
	//Marks a source to make a fadeout
	//Maximum of 3 fadeouts at the same time

	//Local variables
	int i;  //Iterador

	for(i=0;i<3;i++)
	{
		if (!mFadeOutSources[i].used)
		{
			//Posicion no usada
			mFadeOutSources[i].used = true;
			mFadeOutSources[i].source = source;
			break;
		}
	}

	//Return if fadeout is possible
	return (i >= 3);
}

//Set position of source (3d sound)
bool SoundManager::SetSourcePosition( int source, float posx, float posy, float posz)
{
	if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
	{
		ALfloat pos[3] = { posx, posy, posz };	
		alSourcefv(mSources[source].source, AL_POSITION, pos);
		return true;
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::SetSourcePosition(","Can't set source position. Invalid source " + source ,EXCEPTION);
		return false;
	}
}

//Get position of source (3d sound)
bool SoundManager::GetSourcePosition( int source, float& posx, float& posy, float& posz )
{
	if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
	{
		ALfloat pos[3];
		alGetSourcefv(mSources[source].source, AL_POSITION, pos);
		posx = pos[0];
		posy = pos[1];
		posz = pos[2];
		return true;
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::GetSourcePosition(","Can't get source position. Invalid source " + source ,EXCEPTION);
		return false;
	}
}

//Set speed of source (3d source)
bool SoundManager::SetSourceSpeed( int source, float velx, float vely, float velz )
{
	if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
	{
		ALfloat vel[3] = { velx, vely, velz };
		alSourcefv(mSources[source].source, AL_VELOCITY, vel);
		return true;
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::SetSourceSpeed","Can't get source speed. Invalid source " + source ,EXCEPTION);
		return false;
	}
}

//Get speed of source (3d source)
bool SoundManager::GetSourceSpeed(int source, float& velx, float& vely, float& velz)
{
	if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
	{
		ALfloat vel[3];
		alGetSourcefv(mSources[source].source, AL_VELOCITY, vel);
		velx = vel[0];
		vely = vel[1];
		velz = vel[2];
		return true;
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::GetSourceSpeed","Can't set source speed. Invalid source " + source ,EXCEPTION);
		return false;
	}	
}

//Set gain for source
bool SoundManager::SetSourceGain(int source, float gain)
{
	if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
	{
		alSourcef(mSources[source].source, AL_GAIN, gain);
		return true;
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::SetSourceGain","Can't set source gain. Invalid source " + source ,EXCEPTION);
		return false;
	}
}

//Get gain for source
bool SoundManager::GetSourceGain(int source, float& gain)
{
	if (source >= 0 && (unsigned int)source<mSources.size() && mSources[source].reserved)
	{
		alGetSourcef(mSources[source].source, AL_GAIN, &gain);
		return true;
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::GetSourceGain","Can't set source gain. Invalid source " + source ,EXCEPTION);
		return false;
	}
}

//Set listener position (3d sound)
void SoundManager::SetListenerPosition( float posx, float posy, float posz )
{
	alListener3f(AL_POSITION, posx,posy,posz);
}

//Get listener position (3d sound)
void SoundManager::GetListenerPosition(  float& posx, float& posy, float& posz)
{
	ALfloat pos[3];
	alGetListenerfv(AL_POSITION, pos);
	posx = pos[0];
	posy = pos[1];
	posz = pos[2];
}

//Set listener Speed (3d sound)
void SoundManager::SetListenerSpeed( float velx, float vely, float velz )
{
	ALfloat vel[3] = { velx, vely, velz };
	alListenerfv(AL_VELOCITY, vel);	
}

//Get listener Speed (3d sound)
void SoundManager::GetListenerSpeed( float& velx, float& vely, float& velz )
{
	ALfloat vel[3];
	alGetListenerfv(AL_VELOCITY, vel);	
	velx = vel[0];
	vely = vel[1];
	velz = vel[2];
}

//Set listener orientation (3d sound)
void SoundManager::SetListenerOrientation(	float lookatx, float lookaty, float lookatz,
											float upx, float upy, float upz )
{
	// VSpeed of the listener.
	ALfloat ListenerOri[6] = { lookatx, lookaty, lookatz, upx,upy, upz };
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

//Get listener orientation (3d sound
void SoundManager::GetListenerOrientation(	float& lookatx, float& lookaty, float& lookatz,
											float& upx, float& upy, float& upz )
{
	ALfloat ori[6];
	alGetListenerfv(AL_ORIENTATION, ori);	
	lookatx = ori[0];
	lookaty = ori[1];
	lookatz	= ori[2];
	upx	= ori[3];
	upy	= ori[4];
	upz	= ori[5];
}


//---------------PRIVATE FUNCTIONS-------------------------------------------
//TODO: ENABLE OPTIONS FOR AUDIO
bool SoundManager::_initAL()
{
	SingletonLogMgr::Instance()->AddNewLine("SoundManager::InitAL","Initializing sound system...",NORMAL);
	
	ALenum error;
	ALCdevice* pDevice;
	ALCcontext* pContext;

	SingletonLogMgr::Instance()->AddNewLine("SoundManager::InitAL","Opening default sound device...",NORMAL);
	// Get handle to default device.
	pDevice = alcOpenDevice(NULL);

	// Get the device specifier.
	//const ALCubyte* deviceSpecifier = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);

	// Create audio context.
	pContext = alcCreateContext(pDevice, NULL);

	// Set active context.
	alcMakeContextCurrent(pContext);

	// Check for an error.
	if ((error=alcGetError(pDevice)) != ALC_NO_ERROR)
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::InitAL","Can't create openAL context (" + GetALErrorString(error) + ")",EXCEPTION);
		throw(GenericException("Can't create openAL context (" + GetALErrorString(error) + ")",GenericException::LIBRARY_ERROR));
	}

	SingletonLogMgr::Instance()->AddNewLine("SoundManager::InitAL","Sound system initialized correctly",NORMAL);
	return true;


}

void SoundManager::_releaseAL()
{
	bool success = true;

	SingletonLogMgr::Instance()->AddNewLine("SoundManager::releaseAL","Releasing sound system..",NORMAL);
	_clear();
	
	ALCcontext* pCurContext;
	ALCdevice* pCurDevice;

	// Get the current context.
	pCurContext = alcGetCurrentContext();

	// Get the device used by that context.
	pCurDevice = alcGetContextsDevice(pCurContext);

	// Reset the current context to NULL.
	if(!alcMakeContextCurrent(NULL))
		success = false;

	// Release the context and the device.
	alcDestroyContext(pCurContext);
	
	if(!alcCloseDevice(pCurDevice))
		success = false;
	
	if(!success)
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::releaseAL","Sound system released INCORRECLY",EXCEPTION);
	else
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::releaseAL","Sound system released correctly",NORMAL);
	
}

bool SoundManager::_loadSound(const std::string& file, IdBuffer& buffer)
{
	// Variables to load into.
	FILE *fd;
	ALenum format;
	ALenum error;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	
	// Load wav data into buffers.
	alGenBuffers(1, &buffer);

	if((error=alGetError()) != AL_NO_ERROR)
	{	
		alDeleteBuffers(1,&buffer);
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::_loadSound","Error: Can't create openAL Buffer (" + GetALErrorString(error)  + ")",EXCEPTION);	
		return false;	
	}

	// Check if the file exists
	if ((fd = fopen(file.c_str(),"r"))==NULL)
	{
		alDeleteBuffers(1,&buffer);
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::_loadSound","Error: Can't open file " + file,EXCEPTION);			
		return false;
	}
	else
	{
		fclose(fd);
	}

	alutLoadWAVFile((ALbyte*)file.c_str(), &format, &data, &size, &freq, &loop);
	alBufferData(buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	if ((error=alGetError()) != AL_NO_ERROR)
	{		
		alDeleteBuffers(1,&buffer);
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::_loadSound","Error: Can't load sound file " + file + " (" + GetALErrorString(error)  + ")",EXCEPTION);			
		return false;
	}		
	return true;
}

void SoundManager::_clear()
{
	// Delete AL objets
	ALuint aux;	
	std::map<Action, IdBuffer>::iterator itBuffer;
	for( itBuffer = mBuffers.begin(); itBuffer != mBuffers.end(); ++itBuffer )
	{
		aux = (*itBuffer).second;
		alDeleteBuffers (1,&aux);
	}
	
	std::vector<InfoSource>::iterator itSource;
	for( itSource = mSources.begin(); itSource != mSources.end(); ++itSource )
	{
		aux = (*itSource).source;
		alDeleteSources (1,&aux);
	}
	
	mBuffers.clear();
	mSources.clear();
	mMusicActionMap.clear();
}

int SoundManager::_getSource(bool reserved)
{
	// Find a unused source
	int i=0;
	bool found=false;
	ALint state;
	ALenum error;
	
	for( unsigned int i=0; i < mSources.size(); i++)
	{
		if (!mSources[i].reserved)
		{
			// Check if it is being used
			alGetSourcei(mSources[i].source,AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING && state != AL_PAUSED)
			{
				mSources[i].reserved = reserved;
				return i;
			}			
		}
	}
		
	//There is not available source
	//Generate it
	InfoSource info;
	alGenSources(1,&info.source);
	if ((error=alGetError())!=AL_NO_ERROR)
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::_getSource","Can't create source (" + GetALErrorString(error) + ")",EXCEPTION);			
		i=-1;
	}
	else
	{
		info.reserved=reserved;
		mSources.push_back(info);
		i = (int)mSources.size() - 1;	
		std::stringstream ss;
		ss << "Source Created - Number of sources: "<< i + 1;
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::_getSource",ss.str(),DEBUG);			
	}
	
	return i;
}

//Play music
bool SoundManager::PlayMusic(std::string &ActiontoPlay)	
{

	//Find action requested
	std::map<Action, std::string>::iterator it =  mMusicActionMap.find(ActiontoPlay);
	//IF - Action to play not found
	if(it == mMusicActionMap.end())
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::PlayMusic","Action " + ActiontoPlay + " not found to play music" ,EXCEPTION);
		return false;
	}//ELSE - Action already playing
	else if(mMusic)
	{
		//IF - Already playing
		if(
			mMusic->GetFilename() == (*it).second
			&&
			mMusic->IsPlaying()
			)
		{
			return true;
		}//Not playing - so delete music object
		else 
		{
			delete mMusic;
			mMusic = NULL;
		}

	}

	//Create stream to play this music
	mMusic = new OggStream((*it).second);

	//Execute command to play looping
	if(!mMusic->Playback(true))
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::PlayMusic","Not possible to play Ogg music file",EXCEPTION);
		return false;
	}
	else
		return true;
}

//Stop music
void SoundManager::StopMusic()	
{
	if(mMusic)
	{
		mMusic->Stop();
	}
	else
	{
		SingletonLogMgr::Instance()->AddNewLine("SoundManager::StopMusic","Command to stop music when there is no playing",EXCEPTION);
	}
}

std::string SoundManager::GetALErrorString(ALenum err)
{
    switch(err)
    {
        case AL_NO_ERROR:
            return std::string("AL_NO_ERROR");
        break;

        case AL_INVALID_NAME:
            return std::string("AL_INVALID_NAME");
        break;

        case AL_INVALID_ENUM:
            return std::string("AL_INVALID_ENUM");
        break;

        case AL_INVALID_VALUE:
            return std::string("AL_INVALID_VALUE");
        break;

        case AL_INVALID_OPERATION:
            return std::string("AL_INVALID_OPERATION");
        break;

        case AL_OUT_OF_MEMORY:
            return std::string("AL_OUT_OF_MEMORY");
        break;
    };
	return "";
}
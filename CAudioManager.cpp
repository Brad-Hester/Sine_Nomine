#include "CAudioManager.h"

CAudioManager::CAudioManager()
{
	//Initialize everything in the array
	for(int i = 0; i<AUDIO_LAST; i++)
	{
		mauiSourceID[i] = -1;
	}

	//Initialize the manager
	if(!initialize())
	{
		cout << "Error: Failed to initialize audio manager \n";
	}

	loadSounds();
}

bool CAudioManager::initialize()
{
	//Start fresh
	cleanup();

	//If the initialization failed then output the error and return false
	if(!alutInit(NULL, NULL))
	{
		cout << "Error: " << alutGetErrorString(alutGetError());
		return false;
	}

	//Clear Errors
	alGetError();

	alListenerf(AL_GAIN, 1.0f); //Master Volume
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	float f6Orient[] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
	alListenerfv(AL_ORIENTATION, f6Orient);
	
	//Check if the listener initialized correctly
	if(alGetError() != AL_NO_ERROR)
	{
		cout << "Error: Failed to initialize Listener. \n";
		return false;
	}

	//If it initialized successfully return true
	return true;
	
}

void CAudioManager::cleanup()
{
	//Delete all sources
	list<CAudioSource*>::iterator itSource;

	for(itSource=mlAudioSource.begin(); itSource!=mlAudioSource.end(); itSource++)
	{
		delete(*itSource);
	}

	mlAudioSource.clear();

	//Delete all buffers
	list<CAudioBuffer*>::iterator itBuffer;

	for(itBuffer=mlAudioBuffer.begin(); itBuffer!=mlAudioBuffer.end(); itBuffer++)
	{
		delete(*itBuffer);
	}

	mlAudioBuffer.clear();

	alutExit();

	//Reset
	for(int i=0; i<AUDIO_LAST; i++)
	{
		mauiSourceID[i] = -1;
	}
}

bool CAudioManager::createSource(int iSourceID, const string& strFilename)
{
	//Check if the source is already in use
	if(mauiSourceID[iSourceID] != -1)
	{	//If it is delete it
		cout << "Error: Source already in use, deleting contents. \n";
		deleteSource(iSourceID);
	}

	CAudioBuffer* pAudioBuffer = NULL;

	//Check if the file has already been loaded into a buffer
	list<CAudioBuffer*>::iterator it;
	for(it = mlAudioBuffer.begin(); it != mlAudioBuffer.end(); ++it)
	{
		if( (*it)->isThisFile(strFilename))
		{	//The requested file is already loaded into memory
			pAudioBuffer = (*it);
			break;
		}
	}

	//If a buffer isnt found, create one
	if(pAudioBuffer == NULL)
	{
			//Load the file into memory
			pAudioBuffer = new CAudioBuffer();

			//Check if a buffer was created
			if(pAudioBuffer == NULL)
			{
				cout << "Error: Unable to create AudioBuffer. \n";
				return false;
			}
		
		//Call the audio buffer class and call its loadFile method
		if(!pAudioBuffer->loadFile(strFilename))
		{
			//Cannot load file
			delete pAudioBuffer;
			return false;
		}

		//Store the buffer in the list
		mlAudioBuffer.push_back(pAudioBuffer);

	}

	//Create new source and attach it to the audio buffer
	CAudioSource* pAudioSource = new CAudioSource();

	//Check the pointer was created successfully
	if(pAudioSource == NULL)
	{
		cout << "Error: Unable to create AudioSource. \n";

		if(pAudioBuffer->getNumberUsing() < 1)
		{
			delete pAudioBuffer;
			mlAudioBuffer.pop_back();
		}

		return false;
	}

	//Make source, if it fails (return false) delete it
	if(!pAudioSource->create(pAudioBuffer))
	{
		cout << "Error: AudioSource: Create Failed. \n";
		delete pAudioSource;

		if(pAudioBuffer->getNumberUsing() < 1)
		{
			//Delete the buffer
			delete pAudioBuffer;
			//Remove it from the list
			mlAudioBuffer.pop_back();
		}

		return false;
	}

	//Store the Source in the list
	mlAudioSource.push_back(pAudioSource);
	mauiSourceID[iSourceID] = pAudioSource->getID();

	return true;
}

void CAudioManager::deleteSource(int iSourceID)
{
	//Check if the source is valid
	if(!isValid(iSourceID))
	{
		return;
	}

	//Find source in the list
	list<CAudioSource*>::iterator itSource;

	for(itSource = mlAudioSource.begin(); itSource!=mlAudioSource.end(); itSource++)
	{
		if((*itSource)->getID() == mauiSourceID[iSourceID])
		{	//Found it
			unsigned int uiBufferID = (*itSource)->getBufferID();
			
			//Delete Source
			delete (*itSource);
			mlAudioSource.erase(itSource);

			//Reset
			mauiSourceID[iSourceID] = -1;

			//Find buffer in the list
			if(uiBufferID != -1)
			{
				list<CAudioBuffer*>::iterator itBuffer;
				for(itBuffer = mlAudioBuffer.begin(); itSource!=mlAudioSource.end(); itBuffer++)
				{
					if((*itBuffer)->getID() == uiBufferID)
					{
						if((*itBuffer)->getNumberUsing() <1)
						{	//Buffer no longer needed
							delete (*itBuffer);
							mlAudioBuffer.erase(itBuffer);
							return;
						} //If buffer isnt needed
					} //If buffer found
				} //For all buffers
			}// If buffer is loaded

			//Done
			return;
		}
	}


}

void CAudioManager::changeGain(int iSourceID, float fGain)
{
	//Error check both parameters
	if(!isValid(iSourceID) || (fGain < 0))
	{
		//Invalid
		false;
	}

	//If its fine
	alSourcef(mauiSourceID[iSourceID], AL_GAIN, fGain);
}

void CAudioManager::play(int iSourceID, bool bLoop)
{
	//Check if the source is valid
	if(!isValid(iSourceID))
	{
		return;
	}

	//Clear Errors
	alGetError();

	//Set the looping value to the one passed in
	alSourcei(mauiSourceID[iSourceID], AL_LOOPING, (bLoop?1:0)); //Short hand operator will set the value according to bLoop 

	ALenum e = alGetError();
	if(e != AL_NO_ERROR)
	{
		cout << "Error: alSourcei failed with code %d. \n";
		return;
	}

	alSourcePlay(mauiSourceID[iSourceID]);
	e =	alGetError();
	if(e != AL_NO_ERROR)
	{
		cout << "alSourcePlay failed with code %d. \n";
		return;
	}

}

void CAudioManager::stop(int iSourceID)
{
	//Check if its valid
	if(!isValid(iSourceID))
	{
		false;
	}

	alSourceStop(mauiSourceID[iSourceID]);
}

void CAudioManager::pause(int iSourceID)
{
	//Check if its valid
	if(!isValid(iSourceID))
	{
		false;
	}

	alSourcePause(mauiSourceID[iSourceID]);
}

bool CAudioManager::isPlaying(int iSourceID)
{
	//Check if the source is valid
	if(!isValid(iSourceID))
	{
		return false;
	}

	
	int iState; //Store the current state of the source
	//Get the current state of the source from the array
	alGetSourcei(mauiSourceID[iSourceID], AL_SOURCE_STATE, &iState);

	//Return the state
	return (iState == AL_PLAYING);
	
}

void CAudioManager::loadSounds()
{

	if(!createSource(CLICK, "audio/console_tone_1.wav"))
	{
		cout << "Error: Failed to load sound console_tone_1.wav\n";
	}

	if(!createSource(CLICK_FAIL, "audio/console_tone_2.wav"))
	{
		cout << "Error: Failed to load sound console_tone_2.wav\n";
	}

	if(!createSource(MISSILE_LAUNCH, "audio/Missile_Launch.wav"))
	{
		cout << "Error: Failed to load sound Missile_Launch.wav\n";
	}

	if(!createSource(EXPLOSION, "audio/explosion.wav"))
	{
		cout << "Error: Failed to load sound explosion.wav\n";
	}
}
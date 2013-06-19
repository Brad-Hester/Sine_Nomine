#ifndef CAUDIOMANAGER_H
#define CAUDIOMANAGER_H

//#include "CTLManager.h"
#include "Audio Includes.h"
#include "CAudioBuffer.h"
#include "CAudioSource.h"


class CAudioManager
{
private:

	inline bool isValid(int iSourceID); //Check if the source has been initialized
	//List of buffers and sources
	list<CAudioBuffer*> mlAudioBuffer;
	list<CAudioSource*> mlAudioSource;
	//Reference to source ID
	ALuint mauiSourceID[AUDIO_LAST]; //Uses the value of AUDIO_LAST to create the amount of necessary elements

public:
	CAudioManager();
	~CAudioManager(){cleanup();};

	bool initialize(); //Initialize 
	//Clean up memory
	void cleanup(); //Destructor will call this but can be called manually

	//Create a source from the name of the file
	bool createSource(int iSourceID, const string& strFilename);
	void deleteSource(int iSourceID); //Delete when done

	//Play the sound of a specific source, can be looped
	void changeGain(int iSourceID, float fGain); //Change the volume
	void play(int iSourceID, bool bLoop = false);
	void stop(int iSourceID);
	void pause(int iSourceID);
	bool isPlaying(int iSourceID); //Check if the sound is already playing
	void loadSounds(); //List of sounds to be loaded

};


//Define the isValid function in the header as it is inline
inline bool CAudioManager::isValid(int iSourceID)
{
	//Check if the array has a value for the source ID
	int uiSourceID = mauiSourceID[iSourceID];

	//check if the source ID has been initialized
	if(uiSourceID == -1)
	{
		cout << "Error: Source has not been initialized. \n";
		return false;
	}

	//If the source ID is in valid
	if(alIsSource(uiSourceID) == AL_FALSE)
	{
		cout << "Error: Source is invalid. \n";
		return false;
	}

	return true;
}



#endif

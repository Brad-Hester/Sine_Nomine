#include "CAudioSource.h"


bool CAudioSource::create( CAudioBuffer* pAudioBuffer)
{
	//Check if an audiobuffer exists
	if(pAudioBuffer == NULL)
	{
		return false;
	}

	//If the ID is -1 there was an error initializing the buffer
	unsigned int uiBufferID = pAudioBuffer->getID();
	if( uiBufferID == -1)
	{
		return false; 
	}

	//Clear Errors
	alGetError();

	alGenSources(1, &muID);

	//If there was an error
	if(alGetError() != AL_NO_ERROR)
	{
		muID = -1;
		return false;
	}

	//Clear Errors again
	alGetError();

	//Connect this buffer to the source
	alSourcei(muID, AL_BUFFER, uiBufferID);

	//Default Parameters
	alSource3f(muID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSourcef(muID, AL_PITCH, 1.0f);
	alSourcef(muID, AL_GAIN, 1.0f);
	alSourcei(muID, AL_SOURCE_RELATIVE, 1);

	//If there was an error setting up the source
	if(alGetError() != AL_NO_ERROR)
	{
		alDeleteSources(1, &muID);	//Remove the source
		muID = -1;
		return false;
	}

	//Save pointer to buffer
	mpAudioBuffer =	pAudioBuffer;

	//Increment buffer count
	mpAudioBuffer->attachBuffer();

	return true;
}

unsigned int CAudioSource::getBufferID()
{
	if(mpAudioBuffer == NULL)
	{
		return -1;
	}

	return mpAudioBuffer->getID();
}

CAudioSource::~CAudioSource()
{
	//Detatch audio buffer from the source, will delete buffer later
	if(mpAudioBuffer != NULL)
	{
		mpAudioBuffer->detachBuffer();
		mpAudioBuffer = NULL;
	}

	if(muID != -1)
	{
		alSourceStop(muID);
		alDeleteSources(1, &muID);
		muID = -1;
	}
}
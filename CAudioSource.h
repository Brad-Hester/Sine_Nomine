#ifndef CAUDIOSOURCE_H
#define CAUSIOSOURCE_H

#include "Audio Includes.h"
#include "CAudioBuffer.h"


class CAudioSource
{
private:
	CAudioBuffer*  mpAudioBuffer;
	ALuint muID;


public:
	CAudioSource(){mpAudioBuffer = NULL; muID = -1;};
	~CAudioSource();

	bool create( CAudioBuffer* pAudioBuffer);

	unsigned int getID(){return muID;};
	unsigned int getBufferID();


};

#endif
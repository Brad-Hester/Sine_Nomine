#ifndef _CAUDIOBUFFER_H
#define _CAUDIOBUFFER_H

//#include "CTLManager.h"
#include "Audio Includes.h"

class CAudioBuffer
{
private: 
	int mCounter;
	unsigned int muID; //ID of the buffer
	string mstrFilename;

public:
	CAudioBuffer(){mCounter = 0; muID = -1;}; //Constructor
	~CAudioBuffer(); //Destructor

	//Returns true if the filename is the same as the contained filename of the class
	bool isThisFile( const string& strfilename);
	bool loadFile ( const string& strfilename); //Load the file

	int getNumberUsing(){return mCounter;}; //How many sources are attached to this buffer?
	void attachBuffer(){mCounter++;}; //Attach using this method
	void detachBuffer(){mCounter--;}; //Detach using this method

	unsigned int getID(){return muID;}; //Returns the ID of the specific buffer
};




#endif
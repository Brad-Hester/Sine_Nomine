#include "CAudioBuffer.h"

bool CAudioBuffer::isThisFile(const string& strfilename)
{
	//Compare the passed filename with the filename being stored, return true if match
	return (strfilename.compare(mstrFilename) == 0);
}

bool CAudioBuffer::loadFile(const string& strfilename)
{
	//Clear Errors
	alutGetError();

	//Check for invalid filename
	if (strfilename.length() < 5)
	{
		cout << "Error: Invalid file name \n";
		return false;
	}

	//Check if the file exists
	FILE* fp;
	fopen_s(&fp, strfilename.c_str(), "rb"); //Open file as read only binary data
	//If the file can't be found
	if(fp == NULL)
	{
		cout << "Error: No such file exists. \n";
		return false;
	}

	fclose(fp);
	
	//Load the file
	muID = alutCreateBufferFromFile( strfilename.c_str());

	//Check for any errors loading into memory
	if( alutGetError() != ALUT_ERROR_NO_ERROR)
	{
		cout << "Error: Failed to load file \n";
		muID = -1;
		return false;
	}

	//Store the name for future reference
	mstrFilename = strfilename;
	
	return true;

}

CAudioBuffer::~CAudioBuffer()
{
	mstrFilename.clear();

	if(muID != -1)
	{
		alDeleteBuffers(1, &muID);
		muID = -1;
	}

}
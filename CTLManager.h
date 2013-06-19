#ifndef __CTLManager_H
#define __CTLManager_H

#include "TL-Engine.h"
#include "CAudioManager.h"
#include <vector>
#include <string>

using namespace tle;
using namespace std;

class CTLManager
{
	private:
		CTLManager();								// Constructor
		CTLManager(const CTLManager&);				// Copy Constructor
		CTLManager& operator=(const CTLManager&);	// Assignment Operator

		static CTLManager* mpInstance;

		I3DEngine* mpEngine;
		ICamera*   mpCamera;
		CAudioManager* mpAudio;
	public:
		static CTLManager* GetInstance();

		I3DEngine* GetEngine();
		ICamera*   GetCamera();
		CAudioManager* GetAudio();
};


#endif
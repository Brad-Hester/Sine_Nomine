#ifndef __CGame_H
#define __CGame_H

#include "CHexGrid.h"
#include "CTLManager.h"
#include "Utility.h"
#include "CGUI.h"

using namespace tle;

// CGame is a singleton - only ever allowed one instance.
// Have to create an instance through CreateInstance function to make
// sure the class is being used correctly.

class CGame
{
	private:

		// These three operations must all be private to ensure CGame is a singleton

		CGame();						// Constructor
		CGame(const CGame&);			// Copy Constructor
		CGame& operator=(const CGame&); // Assignment Operator

		static CGame* mpInstance;		// Points to existing instance of CGame (or 0)

		CTLManager* mpTLManager;
		I3DEngine*	mpEngine;
		ICamera*	mpCamera;

		CHexGrid*	mpGrid;
		CGUI*		mpGUI;

		IMesh*	    mpMeshFloor;
		IModel*     mpModelFloor;

		EGameState	meGameState;
		CUnit*      mpBusyUnit;

		/// Mouse Picking ///
		CHexNode* PickHex();
	public:
		~CGame();

		static CGame* GetInstance();

		bool RunGameLoop();
};

#endif
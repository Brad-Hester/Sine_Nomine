#ifndef __CEditor_H
#define __CEditor_H

#include "CHexGrid.h"
#include "CTLManager.h"
#include "Utility.h"

using namespace tle;

// CEditor is a singleton - only ever allowed one instance.
// Have to create an instance through CreateInstance function to make
// sure the class is being used correctly.

class CEditor
{
	private:

		// These three operations must all be private to ensure CEditor is a singleton

		CEditor();						// Constructor
		CEditor(const CEditor&);			// Copy Constructor
		CEditor& operator=(const CEditor&); // Assignment Operator

		static CEditor* mpInstance;		// Points to existing instance of CEditor (or 0)

		CTLManager* mpTLManager;
		I3DEngine*	mpEngine;
		ICamera*	mpCamera;

		CHexGrid*	mpGrid;
		deque<CHexNode*>* mpPath;

	public:
		~CEditor();

		static CEditor* GetInstance();

		bool RunGameLoop();

		/// Mouse Picking Functions ///
		CHexNode* PickHex();
};

#endif
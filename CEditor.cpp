#include "CEditor.h"
#include <time.h>

CEditor* CEditor::mpInstance = 0; // Initialise instance pointer

////////////////////////////////////////////////////////////////////////////////
// CEditor::CEditor
////////////////////////////////////////////////////////////////////////////////
// Description:	Constructor for CEditor
// Parameters:	None
////////////////////////////////////////////////////////////////////////////////
CEditor::CEditor()
{
	mpTLManager = CTLManager::GetInstance();
	mpEngine	= mpTLManager->GetEngine();
	mpCamera	= mpTLManager->GetCamera();
	
	mpCamera->SetPosition(0.0f, 10.0f, -10.0f);

	mpGrid = new CHexGrid(2, 2);
	mpPath = 0;

	srand(time(NULL));
}

////////////////////////////////////////////////////////////////////////////////
// CEditor::CEditor
////////////////////////////////////////////////////////////////////////////////
// Description:	Copy constructor for CEditor
// Parameters:	CEditor	- reference to other CEditor
////////////////////////////////////////////////////////////////////////////////
CEditor::CEditor(const CEditor&)
{
	// Woops! This should never be called.
	// Only one instance of CEditor is allowed.
}

////////////////////////////////////////////////////////////////////////////////
// CEditor::~CEditor
////////////////////////////////////////////////////////////////////////////////
// Description:	Deconstructor for CEditor
// Parameters:	None
////////////////////////////////////////////////////////////////////////////////
CEditor::~CEditor()
{
	delete mpGrid;
}

////////////////////////////////////////////////////////////////////////////////
// CEditor::GetInstance
////////////////////////////////////////////////////////////////////////////////
// Description:	Accessor to singleton instance of CEditor
// Parameters:	None
// Returns:		Pointer to either a new instance of CEditor, or the already
//				existing instance of CEditor
////////////////////////////////////////////////////////////////////////////////
CEditor* CEditor::GetInstance()
{
	// Singleton instance does not yet exist - make one
	if(mpInstance == 0)
	{
		mpInstance = new CEditor;
	}

	// Return singleton instance
	return mpInstance;
}

////////////////////////////////////////////////////////////////////////////////
// CEditor::RunGameLoop
////////////////////////////////////////////////////////////////////////////////
// Description:	Runs a SINGLE iteration of the game loop. 
// Parameters:	None
// Returns:		true if game is not over (loop should be called again)
//				false if game is over (loop should not be called again)
////////////////////////////////////////////////////////////////////////////////
bool CEditor::RunGameLoop()
{
	if(mpEngine->IsRunning())
	{
		mpEngine->DrawScene();
		float frameTime = mpEngine->Timer();
		mpGrid->Update(frameTime);

		if(mpEngine->KeyHit(Key_Numpad5))
		{
			mpGrid->SaveToFile();
		}

		if(mpEngine->KeyHit(Key_Numpad0))
		{
			mpPath = 0;
			delete mpGrid;
			mpGrid = new CHexGrid("Levels/1.txt");
		}

		if(mpEngine->KeyHit(Key_Escape))
		{
			mpEngine->Stop();
			mpEngine->Delete();

			return false;
		}

		if(mpEngine->KeyHit(Mouse_LButton))
		{
			if(mpPath != 0)
			{
				for(deque<CHexNode*>::iterator i = mpPath->begin(); i < mpPath->end(); i++)
				{
					(*i)->SetBorderType(NONE);
				}

				delete mpPath;
				mpPath = 0;
			}
			mpGrid->SetFocus(PickHex());
			cout << mpGrid->GetFocus()->GetDistance() << ":" << mpGrid->GetFocus()->GetCost() << endl;
		}

		if(mpGrid->GetFocus() != 0)
		{
			if(mpEngine->KeyHit(Key_Add))
			{
				mpGrid->GetFocus()->AddTerrainHeight(1);
			}
			if(mpEngine->KeyHit(Key_Subtract))
			{
				mpGrid->GetFocus()->AddTerrainHeight(-1);
			}
		}

		if(mpEngine->KeyHit(Mouse_RButton) && mpGrid->GetFocus() != 0)
		{
			if(mpPath != 0)
			{
				for(deque<CHexNode*>::iterator i = mpPath->begin(); i < mpPath->end(); i++)
				{
					(*i)->SetBorderType(NONE);
				}

				delete mpPath;
				mpPath = 0;
			}

			CHexNode* pTarget = PickHex();
			mpPath = mpGrid->FindPath(mpGrid->GetFocus(), pTarget);

			if(mpPath != 0)
			{
				for(deque<CHexNode*>::iterator i = mpPath->begin(); i < mpPath->end(); i++)
				{
					(*i)->SetBorderType(ACTIVE);
				}

				mpGrid->GetFocus()->MoveUnitTo(pTarget);
			}
		}

		// Camera movement - only if window has focus
		if(mpEngine->IsActive())
		{
			if(mpEngine->GetMouseX() <= 0)
			{
				mpCamera->MoveX(-5.0f * frameTime);
			}
			else if(mpEngine->GetMouseX() >= mpEngine->GetWidth())
			{
				mpCamera->MoveX(5.0f * frameTime);
			}

			if(mpEngine->GetMouseY() <= 0)
			{
				mpCamera->MoveZ(5.0f * frameTime);
			}
			else if(mpEngine->GetMouseY() >= mpEngine->GetHeight())
			{
				mpCamera->MoveZ(-5.0f * frameTime);
			}
		}

		CHexNode* pFocus = mpGrid->GetFocus();

		if(pFocus)
		{
			if(mpEngine->KeyHit(Key_1))
			{
				pFocus->AddNewUnit(SOLDIER);
			}
			else if(mpEngine->KeyHit(Key_2))
			{
				pFocus->AddNewUnit(TANK);
			}
			else if(mpEngine->KeyHit(Key_3))
			{
				pFocus->AddNewUnit(AIRCRAFT);
			}
			else if(mpEngine->KeyHit(Key_4))
			{
				pFocus->AddNewUnit(COMMANDER);
			}
		}

		if(mpEngine->KeyHit(Key_Numpad6))
		{
			mpGrid->ExtendGrid(1, 0, true);
		}
		if(mpEngine->KeyHit(Key_Numpad8))
		{
			mpGrid->ExtendGrid(0, 1, true);
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// CEditor::PickHex
////////////////////////////////////////////////////////////////////////////////
// Description:	Finds the node closest to the mouse cursor
// Parameters:	None
// Returns:		Pointer to the node that the mouse cursor is nearest
////////////////////////////////////////////////////////////////////////////////
CHexNode* CEditor::PickHex()
{
	int nearestDist = -1;
	CHexNode* pPickedHex = mpGrid->GetLast();

	int mouseX = mpEngine->GetMouseX();
	int mouseY = mpEngine->GetMouseY();

	CHexNode* pHex = 0;

	for(int gridX = 0; gridX < mpGrid->GetWidth(); gridX++)
	{
		for(int gridY = 0; gridY < mpGrid->GetHeight(); gridY++)
		{
			pHex = mpGrid->GetHex(gridX, gridY);

			int pixelX;
			int pixelY;

			float worldPoint[3] = {pHex->GetWorldX(), pHex->GetWorldY(), pHex->GetWorldZ()};

			if(PixelFromWorldPoint(worldPoint, mpCamera, mpEngine, &pixelX, &pixelY))
			{
				int dist = (pixelX - mouseX) * (pixelX - mouseX) + 
						   (pixelY - mouseY) * (pixelY - mouseY);
				if(nearestDist == -1 || dist < nearestDist && dist)
				{
					nearestDist = dist;
					pPickedHex = pHex;
				}
			}
		}
	}

	return pPickedHex;
}

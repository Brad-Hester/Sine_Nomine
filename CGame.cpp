#include "CGame.h"
#include <time.h>

#include "CSoldier.h"
#include "CTank.h"
#include "CAircraft.h"
#include "CCommander.h"

CGame* CGame::mpInstance = 0; // Initialise instance pointer

////////////////////////////////////////////////////////////////////////////////
// CGame::CGame
////////////////////////////////////////////////////////////////////////////////
// Description:	Constructor for CGame
// Parameters:	None
////////////////////////////////////////////////////////////////////////////////
CGame::CGame()
{
	mpTLManager = CTLManager::GetInstance();
	mpEngine	= mpTLManager->GetEngine();
	mpCamera	= mpTLManager->GetCamera();
	
	mpCamera->SetPosition(0.0f, 10.0f, -10.0f);
	mpGUI = new CGUI();

	mpGrid = 0;

	mpMeshFloor = mpEngine->LoadMesh("Floor.x");
	mpModelFloor = mpMeshFloor->CreateModel(0.0f, -0.5f, 0.0f);
	mpModelFloor->Scale(10.0f);

	meGameState = FRONT_END;
	mpBusyUnit	= 0;

	srand(time(NULL));
}

////////////////////////////////////////////////////////////////////////////////
// CGame::CGame
////////////////////////////////////////////////////////////////////////////////
// Description:	Copy constructor for CGame
// Parameters:	CGame	- reference to other CGame
////////////////////////////////////////////////////////////////////////////////
CGame::CGame(const CGame&)
{
	// Woops! This should never be called.
	// Only one instance of CGame is allowed.
}

////////////////////////////////////////////////////////////////////////////////
// CGame::~CGame
////////////////////////////////////////////////////////////////////////////////
// Description:	Deconstructor for CGame
// Parameters:	None
////////////////////////////////////////////////////////////////////////////////
CGame::~CGame()
{
	delete mpGrid;
	delete mpGUI;
}

////////////////////////////////////////////////////////////////////////////////
// CGame::GetInstance
////////////////////////////////////////////////////////////////////////////////
// Description:	Accessor to singleton instance of CGame
// Parameters:	None
// Returns:		Pointer to either a new instance of CGame, or the already
//				existing instance of CGame
////////////////////////////////////////////////////////////////////////////////
CGame* CGame::GetInstance()
{
	// Singleton instance does not yet exist - make one
	if(mpInstance == 0)
	{
		mpInstance = new CGame;
	}

	// Return singleton instance
	return mpInstance;
}

////////////////////////////////////////////////////////////////////////////////
// CGame::RunGameLoop
////////////////////////////////////////////////////////////////////////////////
// Description:	Runs a SINGLE iteration of the game loop. 
// Parameters:	None
// Returns:		true if game is not over (loop should be called again)
//				false if game is over (loop should not be called again)
////////////////////////////////////////////////////////////////////////////////
bool CGame::RunGameLoop()
{
	if(mpEngine->IsRunning())
	{
		mpEngine->DrawScene();
		float frameTime = mpEngine->Timer();
		
		if(mpGrid)
		{
			mpGrid->Update(frameTime);
		}

		if(mpGUI)
		{
			EGameState eNewState = mpGUI->Update();
			if(eNewState != meGameState)
			{
				if(eNewState == IN_GAME && meGameState == FRONT_END)
				{
					mpGrid = new CHexGrid("Levels/1.txt");
				}

				meGameState = eNewState;
			}

			switch(meGameState)
			{
				case FRONT_END:
				break;
				case PAUSE:
				break;
				case IN_GAME:
					if(mpEngine->KeyHit(Mouse_LButton))
					{
						CHexNode* pPicked = PickHex();

						if(pPicked->GetUnit() != 0)
						{
							mpGrid->SetFocus(PickHex());
							mpGUI->DisplayUnit(pPicked->GetUnit());
						}
						else
						{
							// Deselect
							mpGrid->SetFocus(0);
							mpGUI->DisplayUnit(0);
						}
					}

					if(mpBusyUnit == 0)
					{
						if(mpEngine->KeyHit(Mouse_RButton))
						{
							CHexNode* pFocus  = mpGrid->GetFocus();

							if(pFocus != 0)
							{
								CUnit* pUnit = pFocus->GetUnit();

								if(pUnit != 0)
								{
									CHexNode* pPicked = PickHex();
									CUnit*    pEnemy  = pPicked->GetUnit();
								
									if(pEnemy == 0)
									{
										// There is no unit in the hex already
										if(pUnit->GetMovement() >= pFocus->DistanceTo(pPicked))
										{
											pFocus->MoveUnitTo(pPicked);
											mpGrid->SetFocus(pPicked);
											mpBusyUnit = pUnit;
										}
									}
									else if(pEnemy->GetOwner() != pUnit->GetOwner())
									{
										// Unit occupying square is not on the same team
										pUnit->Attack(pPicked->GetUnit());
										mpBusyUnit = pUnit;
									}
								}
							}
						}
					}
					else
					{
						if(mpBusyUnit->IsFinished())
						{
							mpBusyUnit = 0;
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
				break;
				case SHUTDOWN:
					mpEngine->Stop();
					mpEngine->Delete();

					return false;
				break;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////
// CGame::PickHex
////////////////////////////////////////////////////////////////////////////////
// Description:	Finds the node closest to the mouse cursor
// Parameters:	None
// Returns:		Pointer to the node that the mouse cursor is nearest
////////////////////////////////////////////////////////////////////////////////
CHexNode* CGame::PickHex()
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

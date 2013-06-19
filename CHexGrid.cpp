#include "CHexGrid.h"
#include "Utility.h"
#include <algorithm> // For sort
#include <fstream>
#include <sstream>

#include "CHexPlain.h"
#include "CHexWater.h"
#include "CHexForest.h"

const float HEX_RADIUS = 0.5f;
const float HEX_SIDE_LENGTH = 2 * HEX_RADIUS * sin(DegToRad(90.0));
const float HEX_OFFSET_X = cos(DegToRad(30.0)) * HEX_SIDE_LENGTH;
const float HEX_OFFSET_Y = sin(DegToRad(30.0)) * HEX_SIDE_LENGTH;
const float HEX_WIDTH  = 2 * HEX_OFFSET_X;
const float HEX_HEIGHT = HEX_SIDE_LENGTH + 2 * HEX_OFFSET_Y;

////////////////////////////////////////////////////////////////////////////////
// ExistsIn
////////////////////////////////////////////////////////////////////////////////
// Description:	Checks if a node already exists in a deque
// Parameters:	pHaystack*	- The deque to be searched
//				pNeedle*	- The node to search for
// Returns:		True if the node already exists in the deque, false otherwise
////////////////////////////////////////////////////////////////////////////////
int ExistsIn(deque<CHexNode*>* pHaystack, CHexNode* pNeedle)
{
	for(int i = 0; i < pHaystack->size(); i++)
	{
		if(pNeedle == (*pHaystack)[i])
		{
			return i;
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// CompareScore
////////////////////////////////////////////////////////////////////////////////
// Description:	Compares the pathfinding score between two nodes. Used for the
//				std::sort algorithm
// Parameters:	pNode1*	- The two nodes to be compared
//				pNode2*	^
// Returns:		True if node1 is better than node2, false otherwise
////////////////////////////////////////////////////////////////////////////////
bool CompareScore(CHexNode* pNode1, CHexNode* pNode2)
{
	return pNode1->GetCost() + pNode1->GetDistance() < pNode2->GetCost() + pNode2->GetDistance();
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::CHexGrid
////////////////////////////////////////////////////////////////////////////////
// Description:	Constructor for CHexGrid
// Parameters:	HEX_RADIUS	- distance from center to edge of bounding circle
//				width	- number of hexagons in grid along the X axis
//				height	- number of hexagons in grid along the Y axis
////////////////////////////////////////////////////////////////////////////////
CHexGrid::CHexGrid(unsigned int width, unsigned int height)
{
	mWidth = 0; // Must be initialised to extend grid
	mHeight = 0;

	ExtendGrid(width, height, true);

	mpFocus = 0;
}

CHexGrid::CHexGrid(string filePath)
{
	mWidth = 0; // Must be initialised to extend grid
	mHeight = 0;

	ifstream in(filePath);

	int width;
	int height;

	if(in.good())
	{
		in >> width;
		in >> height;

		ExtendGrid(width, height, false);

		for(int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				int terrainType = PLAIN;
				int terrainHeight = 1;
				int unitType = NO_UNIT;
				int unitOwner = 0;

				in >> terrainType;
				in >> terrainHeight;
				in >> unitType;
				in >> unitOwner;

				switch(terrainType)
				{
					case WATER:
						mGrid[i]->at(j) = new CHexWater(i, j);
					break;
					case FOREST:
						mGrid[i]->at(j) = new CHexForest(i, j);
					break;
					default:
						mGrid[i]->at(j) = new CHexPlain(i, j);
					break;
				}

				CHexNode* pNode = mGrid[i]->at(j);
	
				pNode->SetTerrainHeight(terrainHeight);

				if(unitType != NO_UNIT)
				{
					pNode->AddNewUnit(static_cast<EUnitType>(unitType));
					pNode->GetUnit()->SetOwner(unitOwner);
				}
			}
		}
	}
	
	in.close();

	mpFocus = 0;
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::~CHexGrid
////////////////////////////////////////////////////////////////////////////////
// Description:	Deconstructor for CHexGrid
// Parameters:	None
////////////////////////////////////////////////////////////////////////////////
CHexGrid::~CHexGrid()
{
	for(int x = mWidth-1; x >= 0; x--)
	{
		for(int y = mHeight-1; y >= 0; y--)
		{
			delete mGrid[x]->at(y);
		}
		delete mGrid[x];
	}
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::GetHex
////////////////////////////////////////////////////////////////////////////////
// Description:	Finds a CHexNode at given co-ordinates in the grid
// Parameters:	x	- position of the desired node along the x axis
//				y	- position of the desired node along the y axis
// Returns:		A pointer to the desired CHexNode
////////////////////////////////////////////////////////////////////////////////
CHexNode* CHexGrid::GetHex(unsigned int x, unsigned int y)
{
	return mGrid[x]->at(y);
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::GetFocus
////////////////////////////////////////////////////////////////////////////////
// Description: Fetch the currently focused CHexNode
// Parameters:	None
// Returns:		A pointer to the currently focus CHexNode
////////////////////////////////////////////////////////////////////////////////
CHexNode* CHexGrid::GetFocus()
{
	return mpFocus;
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::GetLast
////////////////////////////////////////////////////////////////////////////////
// Description:	Fetch the node which is at the end of the grid
// Parameters:	None
// Returns:		A pointer to the CHexNode at the end of the grid
////////////////////////////////////////////////////////////////////////////////
CHexNode* CHexGrid::GetLast()
{
	return mGrid[mWidth-1]->at(mHeight-1);
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::GetPath
////////////////////////////////////////////////////////////////////////////////
// Description:	Finds an A* path between two nodes (if there is one)
// Parameters:	None
// Returns:		A pointer to the path stored in a deque
////////////////////////////////////////////////////////////////////////////////
deque<CHexNode*>* CHexGrid::FindPath(CHexNode* pStart, CHexNode* pTarget)
{
	deque<CHexNode*> pOpenList;
	deque<CHexNode*> pClosedList;

	pOpenList.push_back(pStart);

	pStart->SetParent(0);
	pStart->SetCost(0);
	pStart->SetDistance(pStart->DistanceTo(pTarget));

	while(pOpenList.size() > 0)
	{
		CHexNode* pCurrent = pOpenList.front();
		pOpenList.pop_front();

		if(pCurrent == pTarget)
		{
			// Found target
			deque<CHexNode*>* pPath = new deque<CHexNode*>;

			while(pCurrent != pStart)
			{
				pPath->push_front(pCurrent);
				pCurrent = pCurrent->GetParent();
			}
			pPath->push_front(pStart);
			
			return pPath;
		}

		deque<CHexNode*> pChildren;

		int xCurr = pCurrent->GetGridX();
		int yCurr = pCurrent->GetGridY();

		if(xCurr > 0)
		{
			pChildren.push_back(GetHex(xCurr-1, yCurr));
		}
		if(xCurr < mWidth-1)
		{
			pChildren.push_back(GetHex(xCurr+1, yCurr));
		}
		if(yCurr > 0)
		{
			pChildren.push_back(GetHex(xCurr, yCurr-1));
		}
		if(yCurr < mHeight-1)
		{
			pChildren.push_back(GetHex(xCurr, yCurr+1));
		}
		if(yCurr % 2 == 0)
		{
			if(xCurr < mWidth-1 && yCurr < mHeight-1)				// Even row
			{														//     4,5 5,5
				pChildren.push_back(GetHex(xCurr+1, yCurr+1));		// 3,4 4,4 5,4
			}														//     4,3 5,3
			if(xCurr < mWidth-1 && yCurr > 0)		
			{
				pChildren.push_back(GetHex(xCurr+1, yCurr-1));
			}
		}
		else
		{		
			if(xCurr > 0 && yCurr < mHeight-1)						// Odd row
			{														// 2,4 3,4
				pChildren.push_back(GetHex(xCurr-1, yCurr+1));		// 2,3 3,3 4,3
			}														// 2,2 3,2
			if(xCurr > 0 && yCurr > 0)
			{
				pChildren.push_back(GetHex(xCurr-1, yCurr-1));
			}
		}

		if(pChildren.size() > 0)
		{
			for(deque<CHexNode*>::iterator i = pChildren.begin(); i != pChildren.end(); i++)
			{
				int newCost = pCurrent->GetCost() + (*i)->GetTerrainType();

				int existsOpen = ExistsIn(&pOpenList, (*i));
				int existsClosed =  ExistsIn(&pClosedList, (*i));

				int existingCost = (*i)->GetCost();
				int x = (*i)->GetGridX();
				int y = (*i)->GetGridY();

				if((existsOpen >= 0 || existsClosed >= 0) && newCost >= (*i)->GetCost())
				{
					// Do Nothing
				}
				else
				{
					(*i)->SetParent(pCurrent);
					(*i)->SetCost(newCost);
					(*i)->SetDistance((*i)->DistanceTo(pTarget));

					if(existsOpen < 0)
					{
						pOpenList.push_back((*i));
						if(existsClosed >= 0)
						{
							deque<CHexNode*>::iterator pos = pClosedList.begin() + existsClosed;
							pClosedList.erase(pos);
						}
					}

					sort(pOpenList.begin(), pOpenList.end(), CompareScore);
					(*i)->GetCost();
				}
			}
		}

		pClosedList.push_back(pCurrent);
	}

	return 0; // No path
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::GetWidth
////////////////////////////////////////////////////////////////////////////////
// Description:	Accessor for mWidth
// Parameters:	None
// Returns:		Width of the grid
////////////////////////////////////////////////////////////////////////////////
unsigned int CHexGrid::GetWidth()
{
	return mWidth;
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::GetHeight
////////////////////////////////////////////////////////////////////////////////
// Description:	Accessor for mHeight
// Parameters:	None
// Returns:		Height of the grid
////////////////////////////////////////////////////////////////////////////////
unsigned int CHexGrid::GetHeight()
{
	return mHeight;
}

////////////////////////////////////////////////////////////////////////////////
// CHexGrid::SetFocus
////////////////////////////////////////////////////////////////////////////////
// Description:	Sets CNode to be the current focus
// Parameters:	pFocus	- the node to be set to focus
// Returns:		None
////////////////////////////////////////////////////////////////////////////////
void CHexGrid::SetFocus(CHexNode* pFocus)
{
	if(mpFocus != 0)
	{
		mpFocus->SetBorderType(NONE);
	}

	mpFocus = pFocus;
	
	if(mpFocus != 0)
	{
		mpFocus->SetBorderType(ACTIVE);
	}
}

void CHexGrid::SaveToFile(string fileName)
{
	ofstream out;
	
	if(fileName == "~DEFAULT")
	{
		// Make a file with a default (indexed) name.
		stringstream fileName;
		ifstream checkFile;

		int i = 0;
		do
		{
			i++;
			fileName.str(""); // Clear the stringstream
			fileName << "Levels/" << i << ".txt";

			checkFile.open(fileName.str());
			checkFile.close();
		}
		while(checkFile.good());

		out.open(fileName.str());
	}
	else
	{
		out.open(fileName);
	}

	if(out.good())
	{
		out << mWidth << " " << mHeight << endl << endl;

		for(int i = 0; i < mGrid.size(); i++)
		{
			for(int j = 0; j < mGrid[i]->size(); j++)
			{
				out << mGrid[i]->at(j)->GetTerrainType()
						<< " " << mGrid[i]->at(j)->GetTerrainHeight()
						<< endl;
			}
			out << endl;
		}

		out.close();
	}
}

void CHexGrid::ExtendGrid(int x, int y, bool useDefault)
{
	if(x > 0)
	{
		for(int i = 0; i < x; i++)
		{
			mGrid.push_back(new vector<CHexNode*>);
			for(int j = 0; j < mHeight; j++)
			{
				mGrid.back()->push_back(useDefault ? new CHexPlain(mWidth, j) : 0);
			}
			mWidth++;
		}
	}
	else if(x < 0)
	{

	}

	if(y > 0)
	{
		for(int i = 0; i < y; i++)
		{
			for(int j = 0; j < mWidth; j++)
			{
				mGrid[j]->push_back(useDefault ? new CHexPlain(j, mHeight) : 0);
			}
			mHeight++;
		}
	}
	else if(y < 0)
	{

	}
}

void CHexGrid::Update(float frameTime)
{
	for(int i = 0; i < mGrid.size(); i++)
	{
		for(int j = 0; j < mGrid[i]->size(); j++)
		{
			mGrid[i]->at(j)->Update(frameTime);
		}
	}
}
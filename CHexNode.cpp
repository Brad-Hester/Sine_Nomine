#include "CHexNode.h"

#include <math.h>
#include <random>
#include "Utility.h"

#include "CSoldier.h"
#include "CTank.h"
#include "CAircraft.h"
#include "CCommander.h"

const float HEX_RADIUS		= 0.5f;
const float HEX_SIDE_LENGTH = 2 * HEX_RADIUS * sin(DegToRad(90.0));
const float HEX_OFFSET_X	= cos(DegToRad(30.0)) * HEX_SIDE_LENGTH;
const float HEX_OFFSET_Y	= sin(DegToRad(30.0)) * HEX_SIDE_LENGTH;
const float HEX_WIDTH		= 2 * HEX_OFFSET_X;
const float HEX_DEPTH		= HEX_SIDE_LENGTH + 2 * HEX_OFFSET_Y;
const float HEX_HEIGHT		= 0.5f;

// Static
IMesh* CHexNode::mpMeshHex	  = 0;
IMesh* CHexNode::mpMeshBorder = 0;

CHexNode::CHexNode(int gridX, int gridY)
{
	mGridX = gridX;
	mGridY = gridY;

	// Defaults
	mDistance	   = 0;
	mCost		   = 0;
	mTerrainHeight = 0;
	meTerrainType  = PLAIN;
	meBorderType   = NONE;
	mpUnit		   = 0;

	if(mpMeshHex == 0)
	{
		I3DEngine* pEngine = CTLManager::GetInstance()->GetEngine();

		mpMeshHex	 = pEngine->LoadMesh("Hex.x");
		mpMeshBorder = pEngine->LoadMesh("Border.x");
	}

	// Create model

	float xPos = gridX * HEX_WIDTH;
	float zPos = gridY * (HEX_DEPTH - HEX_OFFSET_Y);

	if(gridY % 2 == 0)
	{
		// Even row
		xPos += HEX_OFFSET_X;
	}

	mpModelHex	  = mpMeshHex->CreateModel(xPos, 0.0f, zPos);
	mpModelBorder = 0;

	mpModelHex->RotateY(rand()%6*60);
}

CHexNode::~CHexNode()
{
	mpMeshHex->RemoveModel(mpModelHex);
	if(mpModelBorder != 0)
	{
		mpMeshBorder->RemoveModel(mpModelBorder);
	}
}

float CHexNode::GetWorldX()
{
	return mpModelHex->GetX();
}

float CHexNode::GetWorldY()
{
	return mpModelHex->GetY();
}

float CHexNode::GetWorldZ()
{
	return mpModelHex->GetZ();
}

int CHexNode::GetGridX()
{
	return mGridX;
}

int CHexNode::GetGridY()
{
	return mGridY;
}

int CHexNode::GetTerrainHeight()
{
	return mTerrainHeight;
}

ETerrainType CHexNode::GetTerrainType()
{
	return meTerrainType;
}

CHexNode* CHexNode::GetParent()
{
	return mpParent;
}

int CHexNode::GetCost()
{
	return mCost;
}

int CHexNode::GetDistance()
{
	return mDistance;
}

CUnit* CHexNode::GetUnit()
{
	return mpUnit;
}

void CHexNode::SetParent(CHexNode* pParent)
{
	mpParent = pParent;
}

void CHexNode::SetTerrainHeight(int height)
{
	if(height >= 0 && height != mTerrainHeight)
	{
		mpModelHex->ResetScale();
		mpModelHex->ScaleY(1.0f + 0.2f * height);

		mpModelHex->SetY(HEX_HEIGHT * 0.2f * height);

		if(mpModelBorder != 0)
		{
			mpModelBorder->SetY(HEX_HEIGHT * 0.2f * height + HEX_HEIGHT);
		}

		if(mpUnit != 0)
		{
			mpUnit->MoveY(HEX_HEIGHT * 0.2f * (height - mTerrainHeight));
		}

		mTerrainHeight = height;
	}
}

void CHexNode::SetBorderType(EBorderType eType)
{
	if(eType != meBorderType)
	{
		meBorderType = eType;
		if(eType == ACTIVE)
		{
			mpModelBorder = mpMeshBorder->CreateModel(GetWorldX(), GetWorldY() + HEX_HEIGHT, GetWorldZ());
		}
		else 
		{
			mpMeshBorder->RemoveModel(mpModelBorder);
			mpModelBorder = 0;
		}
	}
}

void CHexNode::SetCost(int cost)
{
	mCost = cost;
}

void CHexNode::SetDistance(int dist)
{
	mDistance = dist;
}

void CHexNode::AddTerrainHeight(int height)
{
	SetTerrainHeight(mTerrainHeight + height);
}

bool CHexNode::AddNewUnit(EUnitType eType)
{
	// Hex must be empty
	if(mpUnit == 0)
	{
		switch(eType)
		{
			case SOLDIER:
				mpUnit = new CSoldier(GetWorldX(), GetWorldY(), GetWorldZ());
			break;
			case TANK:
				mpUnit = new CTank(GetWorldX(), GetWorldY(), GetWorldZ());
			break;
			case AIRCRAFT:
				mpUnit = new CAircraft(GetWorldX(), GetWorldY() + 1.0f, GetWorldZ());
			break;
			case COMMANDER:
				mpUnit = new CCommander(GetWorldX(), GetWorldY() + 0.70f, GetWorldZ());
			break;
		}
		
		return true;
	}

	return false;
}

bool CHexNode::AddUnit(CUnit* pUnit)
{
	if(mpUnit == 0)
	{
		mpUnit = pUnit;

		return true;
	}

	return false;
}

bool CHexNode::MoveUnitTo(CHexNode* pTarget)
{
	if(this->mpUnit != 0 && pTarget->mpUnit == 0)
	{
		pTarget->mpUnit = this->mpUnit;
		mpUnit->MoveTo(pTarget->mpModelHex);
		this->mpUnit = 0;
		return true;
	}

	return false;
}

int CHexNode::DistanceTo(CHexNode* pTarget)
{
	int hexSpace[2];
	int targHexSpace[2];

	hexSpace[X] = this->mGridX - static_cast<int>(ceil(this->mGridY * 0.5f));
    hexSpace[Y] = this->mGridX + static_cast<int>(floor(this->mGridY * 0.5f));
    targHexSpace[X] = pTarget->mGridX - static_cast<int>(ceil(pTarget->mGridY * 0.5f));
    targHexSpace[Y] = pTarget->mGridX + static_cast<int>(floor(pTarget->mGridY * 0.5f));

    int dx = targHexSpace[X] - hexSpace[X];
    int dy = targHexSpace[Y] - hexSpace[Y];
	int dist;

    if (dx * dy >= 0) 
	{ // Both positive or both negative
        dist = max(abs(dx),abs(dy));
    }
	else
	{ // One positive, one negative
        dist = abs(dx) + abs(dy);
    }

	return dist;
}

void CHexNode::Update(float frameTime)
{
	if(mpUnit != 0)
	{
		if(mpUnit->GetHitPoints() <= 0)
		{
			delete mpUnit;
			mpUnit = 0;
		}
		else
		{
			mpUnit->Update(frameTime);
		}
	}
}
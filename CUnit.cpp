#include "CUnit.h"

CUnit::CUnit( int hitPoints, int movement, float attack, float defense, float hitRating )
{
	/*Instance of unit class not needed. Abstract.*/
	mHitPoints = hitPoints;
	mMovement = movement;
	mAttack = attack;
	mDefense = defense;
	mHitRating = hitRating;
	mAttackTarget = 0;
	mMoveTarget = 0;
	mOwner = 0;
	mFinished = true;
}

void CUnit::SetOwner(int owner)
{
	mOwner = owner;
}

void CUnit::SetAttack( float attack )
{
	mAttack = attack;
}

void CUnit::SetDefense( float defense )
{
	mDefense = defense;
}

void CUnit::SetHitpoints( int hitPoints )
{
	mHitPoints = hitPoints;
}

void CUnit::Attack(CUnit* pTarget)
{
	mAttackTarget = pTarget;
	mFinished = false;
}

void CUnit::MoveTo(IModel* pTarget)
{
	mMoveTarget = pTarget;
	mFinished = false;
}

void CUnit::SetMovement( int movement )
{
	mMovement = movement;
}

void CUnit::SetHitRating( float hitRating )
{
	mHitRating = hitRating;
}

// Presuming change in Y due to change in ground level -> so shadow would raise too
void CUnit::MoveY(float y)
{
	mModel->MoveY(y);
	mShadow->MoveY(y);
}

void CUnit::SetPosition(float x, float y, float z)
{
	mModel->SetPosition(x, y, z);
	mShadow->SetPosition(x, y + kSHADOW_LEVEL, z);
}

bool CUnit::IsFinished()
{
	return mFinished;
}

CUnit::~CUnit()
{
}

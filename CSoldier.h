#ifndef _CSoldier
#define _CSoldier

#include "CTLManager.h"
#include "CUnit.h"

//Soldier Constants
const int kSOLDIER_HITPOINTS   = 50;
const int kSOLDIER_MOVEMENT    = 2;
const float kSOLDIER_ATTACK    = 5.0f;
const float kSOLDIER_DEFENSE   = 2.0f;
const float kSOLDIER_HITRATING = 1.0f;

class CSoldier : public CUnit{
private:
	static IMesh* soldierMesh;
public:
	CSoldier( float x, float y, float z );

	~CSoldier();
};

#endif
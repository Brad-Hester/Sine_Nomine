#ifndef _CCommander
#define _CCommander

#include "CUnit.h"

//Aircraft Constants
const int kCOMMANDER_HITPOINTS   = 30;
const int kCOMMANDER_MOVEMENT    = 2;
const float kCOMMANDER_ATTACK    = 25.0f;
const float kCOMMANDER_DEFENSE   = 20.0f;
const float kCOMMANDER_HITRATING = 1.0f;

class CCommander : public CUnit{
private:
	static IMesh* commanderMesh;
public:
	CCommander( float x, float y, float z );

	~CCommander();
};


#endif
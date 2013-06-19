#ifndef _CTank
#define _CTank

#include "CTLManager.h"
#include "CUnit.h"
#include "CExplosion.h"

//Tank constants
const int kTANK_HITPOINTS   = 50;
const int kTANK_MOVEMENT    = 1;
const float kTANK_ATTACK    = 8.0f;
const float kTANK_DEFENSE   = 10.0f;
const float kTANK_HITRATING = 0.5f;

const float kMaxTankHeight    = 0.5f;
const float kMinTankHeight    = 0.0f;
const float kTankShadowScale  = 0.1f;
const float kTankHoverSpeed   = 0.25f;

const unsigned int kMISSILE_COUNT = 2;

const float kMissileZIncrease = 0.5f;
const float kMissileYIncrease = 0.01f;

const float kMaxMissileY = 8.0f;

const float kMissileScale = 0.0075f;

const float kMissileDropSpeed = 20.0f;

class CTank : public CUnit{
private:
	static IMesh* tankMesh;
	static IMesh* missileMesh;

	//Variables for hover function
	float tankHover;
	float tankHoverDir;
	float shadowScaleX;
	float shadowScaleZ;

	//Additonal Variables for Missile
	IModel* mMissileModel[kMISSILE_COUNT];

	float mMissileSpeedZ[kMISSILE_COUNT];
	float mMissileSpeedY[kMISSILE_COUNT];

	bool mTargetting[kMISSILE_COUNT];

	CExplosion* mpExplosion;

	void SetOwner(int owner);

public:
	CTank( float x, float y, float z );
	void Update( float frameTime );

	void SetPosition(float x, float y, float z);

	~CTank();
};

#endif
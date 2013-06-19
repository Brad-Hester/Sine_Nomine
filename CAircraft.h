#ifndef _CAircraft
#define _CAircraft

#include "CUnit.h"

//Aircraft Constants
const int kAIRCRAFT_HITPOINTS   = 50;
const int kAIRCRAFT_MOVEMENT    = 4;
const float kAIRCRAFT_ATTACK    = 10.0f;
const float kAIRCRAFT_DEFENSE   = 10.0f;
const float kAIRCRAFT_HITRATING = 0.25f;

const int kAircraftParticles  = 6;
const float kParticleSpeedZ   = 60.0f;
const float kParticleSpeedY   = 12.0f;

class CAircraft : public CUnit{
private:
	static IMesh* aircraftMesh;
	static IMesh* particleMesh;

	IModel* particle[kAircraftParticles];
public:
	CAircraft( float x, float y, float z );
	void MovePosition( float moveSpeed, float frameTime );
	void Update(float frameTime);

	~CAircraft();
};


#endif
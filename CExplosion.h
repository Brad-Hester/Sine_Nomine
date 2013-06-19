#ifndef __CExplosion_H
#define __CExplosion_H

#include "Utility.h"
#include "CTLManager.h"

const int PARTICLE_COUNT = 100;

const float PARTICLE_SCALE = 0.03f;
const float EXPLOSION_LIFE = 3.0f;

class CExplosion
{
	private:
		static I3DEngine*	  mpEngine;
		static ICamera*		  mpCamera;
		static IMesh*		  mpExplosionMesh;
		static CAudioManager* mpAudio;

		float mTimer;

		IModel* mpExplosion[PARTICLE_COUNT];
		float   mDirection[PARTICLE_COUNT][3];
		float   mVelocity[3];
	public:
		CExplosion(float x, float y, float z);
		~CExplosion();

		float Update(float frameTime);
};

#endif
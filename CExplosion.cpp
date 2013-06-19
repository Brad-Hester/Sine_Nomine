#include "CExplosion.h"

using namespace tle;

I3DEngine*		CExplosion::mpEngine = 0;
ICamera*		CExplosion::mpCamera = 0;
CAudioManager*  CExplosion::mpAudio = 0;
IMesh*			CExplosion::mpExplosionMesh = 0;

CExplosion::CExplosion(float x, float y, float z)
{
	if(mpEngine == 0)
	{
		mpEngine = CTLManager::GetInstance()->GetEngine();
		mpCamera = CTLManager::GetInstance()->GetCamera();
		mpAudio  = CTLManager::GetInstance()->GetAudio();
		mpExplosionMesh = mpEngine->LoadMesh("explosion.x");
	}

	mTimer = EXPLOSION_LIFE;

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		mpExplosion[i] = mpExplosionMesh->CreateModel(x, y, z);
		mpExplosion[i]->Scale(PARTICLE_SCALE);

		float angle  = Randf(0.0f, 2 * PI); // Random angle in circle
		float angle2 = Randf(0.0f, 2 * PI);

		mDirection[i][X] = cosf(angle) * 0.2f; // x direciton
		mDirection[i][Y] = sinf(angle) * 0.2f;  // y direciton
		mDirection[i][Z] = cosf(angle2) * 0.2f; // z direciton
	}

	mpAudio->play(EXPLOSION);
}

CExplosion::~CExplosion()
{
	for(int i = 0; i < PARTICLE_COUNT; i++)
	{
		mpExplosionMesh->RemoveModel(mpExplosion[i]);
	}
}

float CExplosion::Update(float frameTime)
{
	for(int i = 0; i < PARTICLE_COUNT; i++)
	{
		mpExplosion[i]->LookAt( mpCamera );
		mpExplosion[i]->Scale(PARTICLE_SCALE);
				
		mpExplosion[i]->MoveX( mDirection[i][X] * frameTime );
		mpExplosion[i]->MoveY( mDirection[i][Y] * frameTime );
		mpExplosion[i]->MoveZ( mDirection[i][Z] * frameTime );
	}

	mTimer -= frameTime;

	return mTimer;
}
#include "CAircraft.h"

IMesh* CAircraft::aircraftMesh = 0;
IMesh* CAircraft::particleMesh = 0;

CAircraft::CAircraft( float x, float y, float z ) : CUnit( kAIRCRAFT_HITPOINTS, kAIRCRAFT_MOVEMENT, kAIRCRAFT_ATTACK, kAIRCRAFT_DEFENSE, kAIRCRAFT_HITRATING )
{
	if(aircraftMesh == 0)
	{
		aircraftMesh = CTLManager::GetInstance()->GetEngine()->LoadMesh("HawkStarfighter.x");
		particleMesh = CTLManager::GetInstance()->GetEngine()->LoadMesh("sphere.x");
	}

	mModel = aircraftMesh->CreateModel( x, y, z );

	//Aircraft shadow
	mShadow = aircraftMesh->CreateModel( mModel->GetX(), kSHADOW_LEVEL, mModel->GetZ() );
	
	mShadow->SetSkin("black.jpg");
	
	float matrix[4][4];
	mShadow->GetMatrix( &matrix[0][0] );
	matrix[1][0] *= 10.0f;
	matrix[1][1] *= 0.0;
	matrix[1][2] *= 10.0f;

	mShadow->SetMatrix( &matrix[0][0] );

	mModel->Scale(0.3f);
	mShadow->Scale(0.3f);

	//Aircraft Particles /*Needs work here*/
	for (int i = 0; i < kAircraftParticles; i++)
	{
		particle[i] = particleMesh->CreateModel( mModel->GetX(), mModel->GetY(), (mModel->GetZ() - 2.0f + (i * 0.3f)) );
		particle[i]->SetSkin( "shadow.jpg" );
		particle[i]->Scale( 0.006f );
	}
}

void CAircraft::Update( float frameTime )
{
	for (int i = 0; i < kAircraftParticles; i++)
	{
		particle[i]->MoveLocalZ( -kParticleSpeedZ * frameTime );
		particle[i]->MoveLocalY( kParticleSpeedY * frameTime );

		float dist = sqrt(pow(mModel->GetX() - particle[i]->GetX() , 2) + pow(mModel->GetZ() - particle[i]->GetZ() , 2));
		if (dist > 1.5f)
		{
			particle[i]->SetPosition(mModel->GetX(), mModel->GetY(), mModel->GetZ());
			if(mMoveTarget)
			{
				particle[i]->LookAt(mMoveTarget);
				particle[i]->Scale(0.02f);
			}
			particle[i]->MoveLocalZ(-3.0f);
		}
	}
}

void CAircraft::MovePosition( float moveSpeed, float frameTime )
{
	mModel->MoveLocalZ( moveSpeed * frameTime );
	mShadow->MoveLocalZ( moveSpeed * frameTime );
}

CAircraft::~CAircraft()
{

}


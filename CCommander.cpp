#include "CCommander.h"

IMesh* CCommander::commanderMesh = 0;

CCommander::CCommander( float x, float y, float z ) : CUnit( kCOMMANDER_HITPOINTS, kCOMMANDER_MOVEMENT, kCOMMANDER_ATTACK, kCOMMANDER_DEFENSE, kCOMMANDER_HITRATING )
{
	if(commanderMesh == 0)
	{
		commanderMesh = CTLManager::GetInstance()->GetEngine()->LoadMesh("spaceship01.x");
	}
	mModel = commanderMesh->CreateModel( x, y, z );
	mModel->Scale( 0.7f );

	//------------------//
	//Creation of shadow//
	//------------------//
	mShadow = commanderMesh->CreateModel( mModel->GetLocalX(), kSHADOW_LEVEL, mModel->GetLocalZ() );
	
	mShadow->SetSkin("black.jpg");
	mShadow->RotateZ( 180.0f );
	
	float matrix[4][4];
	mShadow->GetMatrix( &matrix[0][0] );
	matrix[1][0] *= 10.0f;
	matrix[1][1] *= 0.0;
	matrix[1][2] *= 10.0f;

	mShadow->SetMatrix( &matrix[0][0] );

	mModel->Scale(0.2f);
	mShadow->Scale(0.15f);

	//shadow->AttachToParent( mModel );
}

CCommander::~CCommander()
{

}
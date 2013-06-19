#include "CSoldier.h"

IMesh* CSoldier::soldierMesh = 0;

CSoldier::CSoldier( float x, float y, float z ) : CUnit( kSOLDIER_HITPOINTS, kSOLDIER_MOVEMENT, kSOLDIER_ATTACK, kSOLDIER_DEFENSE, kSOLDIER_HITRATING )
{
	if(soldierMesh == 0)
	{
		soldierMesh = CTLManager::GetInstance()->GetEngine()->LoadMesh("scifisoldier.x");
	}
	mModel = soldierMesh->CreateModel( x, y, z );

	//------------------//
	//Creation of shadow//
	//------------------//
	mShadow = soldierMesh->CreateModel( mModel->GetX(), kSHADOW_LEVEL, mModel->GetZ() );
	
	mShadow->SetSkin("black.jpg");
	mShadow->RotateZ( 180.0f );
	
	float matrix[4][4];
	mShadow->GetMatrix( &matrix[0][0] );
	matrix[1][0] *= 10.0f;
	matrix[1][1] *= 0.0;
	matrix[1][2] *= 10.0f;

	mShadow->SetMatrix( &matrix[0][0] );

	mModel->Scale(0.005f);
	mShadow->Scale(0.005f);
	mModel->RotateY(180.0f);
	mShadow->RotateY(180.0f);
}

CSoldier::~CSoldier()
{

}
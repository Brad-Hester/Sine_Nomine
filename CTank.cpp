#include "CTank.h"

IMesh* CTank::tankMesh = 0;
IMesh* CTank::missileMesh = 0;

CTank::CTank( float x, float y, float z ) : CUnit( kTANK_HITPOINTS, kTANK_MOVEMENT, kTANK_ATTACK, kTANK_DEFENSE, kTANK_HITRATING )
{
	if(tankMesh == 0)
	{
		tankMesh = CTLManager::GetInstance()->GetEngine()->LoadMesh("HoverTank04.x");
		missileMesh = CTLManager::GetInstance()->GetEngine()->LoadMesh("missile.X");
	}
	mModel = tankMesh->CreateModel( x, y, z );

	//------------------//
	//Creation of shadow//
	//------------------//
	mShadow = tankMesh->CreateModel( mModel->GetX(), y + kSHADOW_LEVEL, mModel->GetZ() );
	
	mShadow->SetSkin("black.jpg");
	mShadow->RotateZ( 180.0f );
	
	mModel->Scale(0.2f);
	mShadow->Scale(0.2f);
	mShadow->ScaleY(0.01f);

	tankHover = 0.0f;
	tankHoverDir = 1.0f;

	for(int i = 0; i < kMISSILE_COUNT; i++)
	{
		mMissileModel[i] = 0;
	}

	mpExplosion = 0;
}

void CTank::Update( float frameTime )
{
	if(mMoveTarget)
	{
		mModel->LookAt(mMoveTarget->GetX(), mModel->GetY(), mMoveTarget->GetZ());
		mShadow->LookAt(mMoveTarget->GetX(), mShadow->GetY(), mMoveTarget->GetZ());

		mModel->Scale(0.2f);
		mShadow->Scale(0.2f);
		mShadow->ScaleY(0.01f);

		mModel->MoveLocalZ(3 * frameTime);
		mShadow->MoveLocalZ(3 * frameTime);

		float dist = sqrt(pow(mMoveTarget->GetX() - mModel->GetX(), 2) + pow(mMoveTarget->GetZ() - mModel->GetZ(), 2));

		if(dist < 0.01f)
		{
			SetPosition(mMoveTarget->GetX(), mMoveTarget->GetY(), mMoveTarget->GetZ());
			mMoveTarget = 0;
			mFinished = true;
		}
	}

	if(mAttackTarget)
	{
		bool missileExists = false;
		for(int i = 0; i < kMISSILE_COUNT; i++)
		{
			if(mMissileModel[i] != 0)
			{
				missileExists = true;
				break;
			}
		}

		if(!missileExists)
		{
			for(int i = 0; i < kMISSILE_COUNT; i++)
			{
				float x = i % 2 == 0 ? 0.25f : -0.25f; // Odd / Even offset

				mMissileModel[i] = missileMesh->CreateModel( mModel->GetX() + x, mModel->GetY(), mModel->GetZ());
				mMissileModel[i]->RotateX( -45.0f );
				mMissileModel[i]->Scale( kMissileScale );

				mTargetting[i] = false;
				mMissileSpeedZ[i] = 1.0f;
				mMissileSpeedY[i] = 1.0f;

				CTLManager::GetInstance()->GetAudio()->play(MISSILE_LAUNCH);
			}
		}
		else
		{
			for(int i = 0; i < kMISSILE_COUNT; i++)
			{
				if(mMissileModel[i] != 0)
				{
					if (mTargetting[i] == false)
					{
						mMissileSpeedZ[i] += kMissileZIncrease;
						mMissileSpeedY[i] += kMissileYIncrease;
						mMissileModel[i]->MoveLocalZ( mMissileSpeedZ[i] * frameTime );
						mMissileModel[i]->MoveY( mMissileSpeedY[i] * frameTime );

						//Once it reaches a certain height look at target and fire down
						if (mMissileModel[i]->GetY() > mModel->GetY() + kMaxMissileY)
						{
							mTargetting[i] = true;
						}
					}
					else
					{
						IModel* pTargetModel = mAttackTarget->GetModel();

						mMissileModel[i]->LookAt( pTargetModel );
						mMissileModel[i]->Scale( kMissileScale );
						mMissileModel[i]->MoveLocalZ( (kMissileDropSpeed / kMissileScale) * frameTime );

						float x, y, z;
						int radius = 1;

						x = pTargetModel->GetX() - mMissileModel[i]->GetX();	
						y = pTargetModel->GetY() - mMissileModel[i]->GetY();	
						z = pTargetModel->GetZ() - mMissileModel[i]->GetZ();

						float collisionDist = sqrt( x*x + y*y + z*z );
						if (collisionDist < radius)
						{					
							// Collision

							mpExplosion = new CExplosion(mMissileModel[i]->GetX(), mMissileModel[i]->GetY(), mMissileModel[i]->GetZ());

							for(int j = 0; j < kMISSILE_COUNT; j++)
							{
								missileMesh->RemoveModel(mMissileModel[j]);
								mMissileModel[j] = 0;
							}

							mFinished = true;
							mAttackTarget->SetHitpoints(0);
							mAttackTarget = 0;

							break;
						}
					}
				}
			}
		}
	}

	tankHover += kTankHoverSpeed * tankHoverDir * frameTime;
	mModel->MoveLocalY(kTankHoverSpeed * tankHoverDir * frameTime);

	mShadow->ScaleX(kTankShadowScale * tankHoverDir * frameTime + 1);
	mShadow->ScaleZ(kTankShadowScale * tankHoverDir * frameTime + 1);

	if(tankHover > kMaxTankHeight || tankHover < 0.0f)
	{
		tankHoverDir *= -1.0f; // Inverse direction
	}

	// Update explosion if there is one
	if(mpExplosion != 0)
	{
		if(mpExplosion->Update(frameTime) <= 0)
		{
			// Life expired, delete and reset pointer.
			delete mpExplosion;
			mpExplosion = 0;
		}
	}
}

void CTank::SetPosition(float x, float y, float z)
{
	mModel->SetPosition(x, y + tankHover, z);
	mShadow->SetPosition(x, y + kSHADOW_LEVEL, z);
}

CTank::~CTank()
{
	tankMesh->RemoveModel(mModel);
	tankMesh->RemoveModel(mShadow);
}

void CTank::SetOwner(int owner)
{
	mOwner = owner;

	stringstream ss;
	ss << "HoverTank04_" << owner << ".jpg";

	try
	{
		mModel->SetSkin(ss.str());
	}
	catch(...)
	{
		// Error most likely caused due to not being a skin for the number owner
		// Resort to default skin
		mModel->SetSkin("HoverTank04_0.jpg");
	}
}
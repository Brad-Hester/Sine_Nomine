#ifndef _CUnit
#define _CUnit

#include "CTLManager.h"

enum EUnitType{NO_UNIT, SOLDIER, TANK, AIRCRAFT, COMMANDER};

const float kSHADOW_LEVEL = 0.01f;

class CUnit{
private:
	int mHitPoints;
	int mMovement;
	float mAttack;
	float mDefense;
	float mHitRating;
protected:
	IModel* mModel;
	IModel* mShadow;
	CUnit* mAttackTarget;
	IModel* mMoveTarget;
	bool mFinished; // Has completed animation?
	int mOwner;
public:
	CUnit( int hitPoints, int movement, float attack, float defense, float hitRating );
	void SetHitpoints( int hitPoints );
	void SetMovement ( int movement );
	void SetAttack   ( float attack );
	void SetDefense  ( float defense );
	void SetHitRating( float hitRating );
	virtual void SetOwner ( int owner);

	int		GetHitPoints() { return mHitPoints; };
	int		GetMovement()  { return mMovement; };
	float	GetAttack()    { return mAttack; };
	float	GetDefense()   { return mDefense; };
	float	GetHitRating() { return mHitRating; };
	IModel* GetModel()	   { return mModel; };
	int		GetOwner()	   { return mOwner; };

	virtual void Update( float frameTime ) {};
	virtual void ParticleEffect( float frameTime ) {} ;
	virtual void SetPosition(float x, float y, float z);
	virtual void MoveY(float y);

	virtual void MovePosition( float moveSpeed, float frameTime ) {} ;

	void Attack(CUnit* pTarget);
	void MoveTo(IModel* pTarget);
	bool IsFinished();

	virtual ~CUnit();
};





#endif


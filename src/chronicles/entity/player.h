#ifndef _PLAYER_H
#define _PLAYER_H

#include "character.h"
#include "../../engine/entity/inputtingObj.h"
#include "../../engine/sound/wavesound.h"
#include "../../engine/graphics/texture.h"

#define MAX_PLAYER_HEALTH	12
//#define MAX_PLAYER_HEALTH	1

#define PLAYER_SPEED		57.0f
#define PLAYER_ROT_SPEED	3.0f
//#define PLAYER_ROT_SPEED	4.0f
#define PLAYER_OBJ_TYPE		1
#define PLAYER_LEVITATE_OBJ	12

#define PLAYER_SWORD_FORCE	15.0f
#define SWORD_OBJ_TYPE			5
#define SWORD_BLOCK_OBJ_TYPE	8

#define SWING_DELAY			0.3f
#define	SWING_ROT			120.0f
#define SWING_SPEED			400.0f
#define SWORD_DAMAGE		2

#define PLAYER_HEIGHT		4.0f

class CPlayer: public CCharacter, public IInputtingObj
{
public:
	CPlayer();
	~CPlayer();

	void Levitate();

	void Update( float );
	void Render();
	void RenderAlpha();
	CMousePosition GetMouse();
	void StopSwing();

	void DeathEffect();

	int NumKeys();
	void AddKey();
	bool RemoveKey();


protected:
	void Swing();
	void PhysicalSword();

	bool mSwinging;
	float mSwingRot;
	CPTriangle* mSword;
	CTexture* mSwordTexture;
	CTexture* mSwordMask;
	CTimer lastSwing;
	CTimer mLevitateTimer;
	
	bool mJumping;

	SVector3	mLastSafe;
	CWaveSound* mSwordSwishSound;
	bool mSwingRight;
	//GLUquadricObj* mQuadric;

	bool mLevitating;
	int mNumKeys;
};

#endif

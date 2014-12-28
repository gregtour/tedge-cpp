#include <math.h>
#include "bullet.h"
#include "../../engine/graphics/model.h"
#include "../game/game.h"
#include "../../engine/physics/svector.h"
#include "particles/laserBlast.h"

const char* BULLET_MODEL_FILE	= "bullet.ms3d";


CBullet::CBullet(SVector3 color)
{
	mModel = CManagedModel::Load( &gResourceManager, BULLET_MODEL_FILE );
	mLife = BULLET_LIFE;
	mColor = color;
}

CBullet::~CBullet()
{
}

void CBullet::Update(float dt)
{
	mLife -= dt;
	if (mLife < 0.0f)
		Destroy();
}

void CBullet::Explode()
{
	gEntities.Add(new CLaserBlast(mObject->GetPosition(), mColor.x*2.0f, mColor.y*2.0f, mColor.z*2.0f, 0.25f, 1));
}

void CBullet::Render()
{
	SVector3 pos = mObject->GetPosition();
	{
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		if ( (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) + (z-pos.z)*(z-pos.z) > VIEW_DISTANCE_SQUARED )
			return;
	}

	glTranslatef( pos.x, pos.y, pos.z );
	glRotatef(mRot * DEGREES, 0.0f, -1.0f, 0.0f );
	
	glColor4f(mColor.x, mColor.y, mColor.z, mLife/BULLET_LIFE);
	//glDisable(GL_CULL_FACE);
	//glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	mModel->Render();

	glDisable(GL_BLEND);

}


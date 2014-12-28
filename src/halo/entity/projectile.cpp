#include <math.h>
#include "projectile.h"
#include "../../engine/graphics/model.h"
#include "../game/game.h"
#include "../../engine/physics/svector.h"
#include "particles/laserBlast.h"

const char* PROJECTILE_MODEL_FILE	= "shell.ms3d";


CProjectile::CProjectile(SVector3 color)
{
	mModel = CManagedModel::Load( &gResourceManager, PROJECTILE_MODEL_FILE );
	mLife = PROJECTILE_LIFE;
	mColor = color;
}

CProjectile::~CProjectile()
{
}

void CProjectile::Update(float dt)
{
	mLife -= dt;
	if (mLife < 0.0f)
		Destroy();
}

void CProjectile::Explode()
{
}

void CProjectile::Render()
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
	
	glColor4f(mColor.x, mColor.y, mColor.z, mLife/PROJECTILE_LIFE);
	//glDisable(GL_CULL_FACE);
	//glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	mModel->Render();

	glDisable(GL_BLEND);

}


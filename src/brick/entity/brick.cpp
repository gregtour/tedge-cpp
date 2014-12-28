#include <math.h>
#include "brick.h"
#include "../../engine/graphics/model.h"
#include "../game/game.h"
#include "../../engine/physics/svector.h"

const char* BRICK_MODEL_FILE	= "brick.ms3d";

SVector3 BRICK_MIN = SVector3(0.0f, -1.0f, 0.0f);
SVector3 BRICK_MAX = SVector3(4.0f, 1.0f, 1.0f);


CBrick::CBrick(SVector3 pos, SVector3 color, int type)
{
	mModel = CManagedModel::Load( &gResourceManager, BRICK_MODEL_FILE );
	mColor = color;
	mType = type;

	SSphere bricksphere;
	bricksphere.position = (BRICK_MIN + BRICK_MAX) * 0.5f;
	bricksphere.radius = (BRICK_MAX - BRICK_MIN).Length() * 0.5f;

	SBoundingBox brickbox;
	brickbox.max = BRICK_MIN;
	brickbox.min = BRICK_MAX;

	CreatePhysical(gWorld);
	CPObject* obj = GetPhysicalObject();
	obj->SetPosition(pos);
	obj->SetObjectType(BRICK_OBJECT_TYPE);
	obj->SetFriction(0.0f);
	obj->SetBoundingBox(brickbox);
	obj->SetSphere(bricksphere);
	obj->SetCollisionMask(1);
}

CBrick::~CBrick()
{
}

void CBrick::Update(float dt)
{
}

void CBrick::RenderOrtho()
{
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		glScalef(1.0f/40.0f, 1.0f/40.0f, 1.0f/40.0f);

		glTranslatef( pos.x, pos.y, pos.z );
		glColor3f(mColor.x, mColor.y, mColor.z);
		mModel->Render();

		glScalef(40.0f, 40.0f, 40.0f);
	}
}


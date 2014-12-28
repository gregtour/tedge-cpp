#include "health.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "../../engine/sound/wavesound.h"
#include "racer.h"

int CHealth::mMagic = 0;

const char* HEALTH_TEXTURE_FILE = "cake.png";
const char* HEALTH_MODEL_FILE = "cupcake.ms3d";
const char* HEALTH_SOUND_FILE = "pop.wav";

CHealth::CHealth()
{
    mTime = 0.0f;
	mTexture = CManagedTexture::Load(&gResourceManager, HEALTH_TEXTURE_FILE);
	mModel = CManagedModel::Load(&gResourceManager, HEALTH_MODEL_FILE);
	mPickupSound = CManagedWaveSound::Load(&gResourceManager, HEALTH_SOUND_FILE);
	mType = TYPE_HEALTH;
}

CHealth::~CHealth()
{
}

void CHealth::PickedUp(CRacer* by)
{
	CPickup::PickedUp(by);
}

extern CModel* balloons[4];


void CHealth::Update( float dt )
    {
    mTime += dt;
    CPickup::Update(dt);
    }

void CHealth::Render()
    {
    if (!mMagic)
        CPickup::Render();
    else
        {
	if (mObject)
        for (int b = 0; b < 25; b++)
	    {
        //if (b)
            {
            glPopMatrix();
            glPushMatrix();
            }

		SVector3 pos = mObject->GetPosition();
		glClearColor(1.0f, 0.65f+(pos.y/300.0f), (pos.x/500.0f), 1.0f);

		// EnableLighting( &gLights, pos.x, pos.y, pos.z );
        glDisable(GL_LIGHTING);

		glTranslatef( pos.x, pos.y, pos.z );
		glRotatef(mRot * DEGREES + 30.0f, 0.0f, -1.0f, 0.0f );
        glTranslatef(-2.5f, 0.7f, -1.5f);
        glRotatef(30.0f*(b/3), 0.0f, 1.0f, 0.0f);
        glRotatef(30.0f*(b-1), 1.0f, 0.0f, 0.0f);

        glScalef(0.05f, 0.05f, 0.05f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float colors[5][3] = {{1.0f, 0.1f, 0.2f}, {0.2f, 0.1f, 1.0f}, {0.2f, 0.8f, 0.1f}, {0.8f, 0.2f, 0.6f},  {0.2f,0.8f, 0.6f}};
        glColor4f(colors[b%5][0], colors[b%5][1], colors[b%5][2], 1.0f);
/*
        if (b == 0)
		    glColor4f(1.0f, 0.1f, 0.2f, 1.0f);
        if (b == 1)
		    glColor4f(0.2f, 0.1f, 1.0f, 1.0f);
        if (b == 2)
		    glColor4f(0.2f, 0.8f, 0.1f, 1.0f);*/
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glDisable(GL_CULL_FACE);
		//glDepthMask(GL_FALSE);

		//mTexture->Bind();
        balloons[((int)(5.0f*mTime+b*1.3f))%4]->Render();
		//mModel->Render();
		//mTexture->Unbind();

	}


        }
    }


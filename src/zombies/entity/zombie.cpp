#include "zombie.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "../../engine/graphics/buffer/I3dVertBuf.h"
#include "../../engine/graphics/buffer/I3dIndxBuf.h"
#include <cmath>
#include "anim/zombie.h"

I3DVERTBUF*  vb = NULL;
I3DINDXBUF** indices = NULL;

int FRAMES = 0;
int VERTICES = 0;
int REFERENCES = 0;

CZombie::CZombie()
{
	//mModel = CManagedModel::Load(&gResourceManager, "player.ms3d");
    mTexture = CManagedTexture::Load(&gResourceManager, "zombie.jpg");
	mHasJumped = false;
    t = gEntities.length * 0.576f;
    
    if (!vb) 
    {
        int i, v, f;
        FRAMES = sizeof(ZOMBIE_DATA)/sizeof(ZOMBIE_DATA[0]);
        VERTICES = sizeof(ZOMBIE_DATA[0])/sizeof(double)/3;

        printf("%i FRAMES, %i VERTICES, %i TRIANGLES\n", FRAMES, VERTICES, 2*VERTICES/4);

        indices = new I3DINDXBUF*[FRAMES];

        vb = new I3DVERTBUF(true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_UV1, VERTICES * FRAMES);
    	vb->Lock(VERTICES * FRAMES);
        
        for (f = 0; f < FRAMES; f++) {
            for (v = 0; v < VERTICES*3; v = v + 12) {
                SVector3 a(ZOMBIE_DATA[f][v], ZOMBIE_DATA[f][v+1], ZOMBIE_DATA[f][v+2]);
                SVector3 b(ZOMBIE_DATA[f][v+3], ZOMBIE_DATA[f][v+4], ZOMBIE_DATA[f][v+5]);
                SVector3 c(ZOMBIE_DATA[f][v+6], ZOMBIE_DATA[f][v+7], ZOMBIE_DATA[f][v+8]);
                SVector3 d(ZOMBIE_DATA[f][v+9], ZOMBIE_DATA[f][v+10], ZOMBIE_DATA[f][v+11]);

                SVector3 e(ZOMBIE_DATA[0][v], ZOMBIE_DATA[0][v+1], ZOMBIE_DATA[0][v+2]);
                SVector3 f(ZOMBIE_DATA[0][v+3], ZOMBIE_DATA[0][v+4], ZOMBIE_DATA[0][v+5]);
                SVector3 g(ZOMBIE_DATA[0][v+6], ZOMBIE_DATA[0][v+7], ZOMBIE_DATA[0][v+8]);
                SVector3 h(ZOMBIE_DATA[0][v+9], ZOMBIE_DATA[0][v+10], ZOMBIE_DATA[0][v+11]);

                SVector3 normal;
                normal = CrossProduct(c - a, c - b);
                normal.Normalize();

                vb->SetPos((VECTOR3*)&a);
                vb->SetNormal((VECTOR3*)&normal);
                vb->SetUV0(atan2(e.x,e.z), e.y);
                vb->NextVert();

                vb->SetPos((VECTOR3*)&b);
                vb->SetNormal((VECTOR3*)&normal);
                vb->SetUV0(atan2(f.x,f.z), f.y);
                vb->NextVert();

                vb->SetPos((VECTOR3*)&c);
                vb->SetNormal((VECTOR3*)&normal);
                vb->SetUV0(atan2(g.x,g.z), g.y);
                vb->NextVert();

                vb->SetPos((VECTOR3*)&d);
                vb->SetNormal((VECTOR3*)&normal);
                vb->SetUV0(atan2(h.x,h.z), h.y);
                vb->NextVert();
            }
        }
        vb->Unlock();

        //indices = new I3DINDXBUF(false, numIndices);
        for(i = 0; i < FRAMES; i++) {
            indices[i] = new I3DINDXBUF(false, 6*VERTICES/4);
            indices[i]->Lock();
            for (v = 0; v < VERTICES; v+=4) {
                indices[i]->AddQuad(i*VERTICES + v,
                                    i*VERTICES + v+1,
                                    i*VERTICES + v+2,
                                    i*VERTICES + v+3);
            }
            indices[i]->Unlock();
        }
    }
    REFERENCES++;
}

CZombie::~CZombie()
{
    int i;
    REFERENCES--;
    if (REFERENCES == 0 && vb)
    {
        for (i = 0; i < FRAMES; i++)
        {
            delete indices[i];
            indices[i] = 0l;
        }
        delete vb;
        delete[] indices;
        vb = 0l;
        indices = 0l;
    }
}

void CZombie::Update( float dt )
{
    t += dt;
    MoveForward(dt*0.2f);
    if (sinf(dt) > 0) TurnLeft(dt*0.1f);
    else TurnRight(dt*0.1f);

	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		SVector3 cam = gCamera->GetPosition();

		if (pos.y < -30.0f)
			ResetPosition();
	}

	SetGrounded(false);
}

void CZombie::Render()
{
	// forward vector from model_view_matrix
    float camLook[3];
	float fx, fy, fz;
/*	fx = model_view_matrix.z.x;
	fy = model_view_matrix.z.y;
	fz = model_view_matrix.z.z;*/
	float cx, cy, cz;
	gCamera->GetPosition(&cx, &cy, &cz);
	gCamera->GetLookAt(&camLook[0], &camLook[1], &camLook[2]);
	fx = camLook[0] - cx;
	fy = camLook[1] - cy;
	fz = camLook[2] - cz;
    
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		{
			float x, y, z;
			gCamera->GetPosition( &x, &y, &z );
			if ( (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) + (z-pos.z)*(z-pos.z) > VIEW_DISTANCE_SQUARED )
				return;
            if (((pos.x - cx)*fx + (pos.y - cy)*fy + (pos.z - cz)*fz) < 0.0f)
                return;
		}

		glTranslatef( pos.x, pos.y, pos.z );
        glRotatef(270, 0.0f, 0.0f, 1.0f);
		glRotatef(mRot * DEGREES, 1.0f, 0.0f, 0.0f );

        glRotatef(90, 0.0f, -1.0f, 0.0f);

		glColor3f(1.8f, 1.8f, 1.8f);
        mTexture->Bind();

		//mModel->Render();

    //int frame = 0;//((int)t)%42;
    int frame = (int)(t*5) % 8 + 4;
/*
{
    glScalef(2.0f, 2.0f, 2.0f);
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.5, 0.5);
        for (int i = 0; i < 545*3*4 - 4; i+=3) 
            glVertex3f(ZOMBIE_DATA[frame][i], ZOMBIE_DATA[frame][i+1], ZOMBIE_DATA[frame][i+2]);
    glEnd();
}
*/
        I3dSelectBuffers(vb, indices[frame]);
        I3dRenderPrimIdx2(0, 2*VERTICES/4, 6*VERTICES/4);

        mTexture->Unbind();
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void CZombie::TurnRight(float dt)
{
	mRot += 2.0f * dt;
}

void CZombie::TurnLeft(float dt)
{
	TurnRight(-dt);
}

void CZombie::MoveForward(float dt)
{
	SVector3 force;
	force.x = -cos(mRot) * 180.0f * dt;
	force.z = -sin(mRot) * 180.0f * dt;
	mObject->ApplyForce(force);
}

void CZombie::MoveBackward(float dt)
{
	MoveForward(dt * -0.7f);
}

void CZombie::SetGrounded(bool grounded)
{
	if (grounded)
		mHasJumped = false;
	mGrounded = grounded;
}

void CZombie::SetSafe()
{
	mSafePos = mObject->GetOldPosition();
    mGrounded = true;
    mHasJumped = false;
}

void CZombie::Jump()
{
	if (mGrounded)
	{
		mHasJumped = true;
		SVector3 force;
		force.y = 43.0f;
		mObject->ApplyForce(force);
	}
}

void CZombie::ResetPosition()
{
	mObject->SetPosition(mSafePos);
	mObject->SetVelocity(SVector3(0.0f, 0.0f, 0.0f));
}

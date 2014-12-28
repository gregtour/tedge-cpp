
#include <cmath>

#include "track.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "../../engine/graphics/buffer/I3dVertBuf.h"
#include "../../engine/graphics/lighting.h"


const char*	LEVEL_GROUND_TEXTURE = "grass.jpg";

#ifdef 		_PSP
#define		SOFTWARE_CLIPPING
#include "../../engine/graphics/clipping/math.h"
#include "../../engine/graphics/clipping/clipping.h"

extern ScePspFMatrix4 	model_view_matrix;
extern ScePspFMatrix4 	projection_view_matrix;
extern ScePspFMatrix4 	projection_view_model_matrix;
#endif




void AddPoint(SVector3* p, SVector3* normal, I3DVERTBUF* verts)
{
	verts->SetPos((VECTOR3*)p);
	verts->SetNormal((VECTOR3*)normal);
	if (normal->y == 0.0f)
		verts->SetUV0((p->x + p->z) / 5.0f, p->y / 5.0f);
	else
		verts->SetUV0(p->x / 5.0f, p->z / 5.0f);
	verts->NextVert();	
}

void MakeQuad(SVector3 p1, SVector3 p2, SVector3 p3, SVector3 p4, I3DVERTBUF* verts, CPhysics* phys)
{
	// add plane into physics
	CPTriangle* mTri1 = new CPTriangle(true);
	CPTriangle* mTri2 = new CPTriangle(true);
	
	/*
	SVector3 p2 = p1;
	SVector3 p3 = p4;
	if (p1.y != p4.y)
	{
		p2.y = p4.y;
		p3.y = p1.y;
	} else {
		p2.x = p4.x;
		p3.x = p1.x;
	}*/

	SVector3 normal = ((p1-p2).CrossProduct(p1-p4));
	normal.Normalize();

	mTri1->SetPoint(p1, 0);
	mTri1->SetPoint(p2, 1);
	mTri1->SetPoint(p3, 2);
	mTri2->SetPoint(p1, 0);
	mTri2->SetPoint(p3, 1);
	mTri2->SetPoint(p4, 2);

	AddPoint(&p1, &normal, verts);
	AddPoint(&p2, &normal, verts);
	AddPoint(&p3, &normal, verts);
	AddPoint(&p1, &normal, verts);
	AddPoint(&p3, &normal, verts);
	AddPoint(&p4, &normal, verts);

	mTri1->SetCollisionMask(1|2);
	mTri2->SetCollisionMask(1|2);
	phys->Add(mTri1);
	phys->Add(mTri2);
}


void MakeBox(SVector3 min, SVector3 max, I3DVERTBUF* verts, CPhysics* phys)
{
	float xs[2] = {min.x, max.x};
	float ys[2] = {min.y, max.y};
	float zs[2] = {min.z, max.z};
	SVector3 p[8];

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				p[i*4+j*2+k] = SVector3(xs[i], ys[j], zs[k]);
	MakeQuad(p[0], p[1], p[3], p[2], verts, phys);
	MakeQuad(p[4], p[5], p[7], p[6], verts, phys);
	MakeQuad(p[0], p[1], p[5], p[4], verts, phys);
	MakeQuad(p[2], p[3], p[7], p[6], verts, phys);
	MakeQuad(p[1], p[3], p[7], p[5], verts, phys);
	MakeQuad(p[0], p[2], p[6], p[4], verts, phys);

}


int NUM_BLOCKS = 50;
CModel* blocks;

CTrack::CTrack()
{
    //BuildTrack();
	mTrackModel = new CModel("track.ms3d");
	gWorld->Add(mTrackModel->GetVerts(), 1, 1|2);
    //gWorld->SortStaticObjects(20, 4, 20);
	mTexture = CManagedTexture::Load(&gResourceManager, LEVEL_GROUND_TEXTURE);

	I3DVERTBUF* verts = new I3DVERTBUF(true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_UV1, (NUM_BLOCKS*6*2)*3);
	verts->Lock((NUM_BLOCKS*6*2)*3);

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		int x = rand()%39;
		int z = rand()%39;
        int y = rand()%39 + 20.0f;
		MakeBox(SVector3(x*5.0f, y+0.0f, z*5.0f), SVector3((x+1)*5.0f, y+5.0f, (z+1)*5.0f), verts, gWorld);
	}

	verts->Unlock();
    blocks = new CModel(verts);
}


CTrack::~CTrack()
    {
	    if (mTrackModel)
	    {
	    delete mTrackModel;
	    mTrackModel = NULL;
	    }
    }

void CTrack::BuildTrack()
    {
    float heights[100][100];

    for (int x = 0; x < 100; x++)
        for (int y = 0; y < 100; y++)
            heights[x][y] = (x + y) * 0.02f + (y-50)*(y-50)*0.03f + 10.0f/((x-50)*(x-50)+1.0f);

    I3DVERTBUF* buf = new I3DVERTBUF(false, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_UV1, 99*99*2*3);
    buf->Lock();
    for (int x = 0; x < 99; x++)
        for (int y = 0; y < 99; y++)
            {
            SVector3 i(1.0f, heights[x+1][y] - heights[x][y], 0.0f);
            SVector3 j(0.0f, heights[x][y+1] - heights[x][y], 1.0f);
            SVector3 k = i.CrossProduct(j);
            k.Normalize();

            buf->SetPos(x*10.0f, heights[x][y], y*10.0f);
            buf->SetNormal(k.x, k.y, k.z);
            buf->SetUV0(x, y);
            buf->NextVert();

            buf->SetPos((x+1)*10.0f, heights[x+1][y], y*10.0f);
            buf->SetNormal(k.x, k.y, k.z);
            buf->SetUV0(x+1, y);
            buf->NextVert();

            buf->SetPos(x*10.0f, heights[x][y+1], (y+1)*10.0f);
            buf->SetNormal(k.x, k.y, k.z);
            buf->SetUV0(x, y+1);
            buf->NextVert();

            buf->SetPos((x+1)*10.0f, heights[x+1][y], y*10.0f);
            buf->SetNormal(k.x, k.y, k.z);
            buf->SetUV0(x+1, y);
            buf->NextVert();

            buf->SetPos((x+1)*10.0f, heights[x+1][y+1],(y+1)*10.0f);
            buf->SetNormal(k.x, k.y, k.z);
            buf->SetUV0(x+1, y+1);
            buf->NextVert();

            buf->SetPos((x)*10.0f, heights[x][y+1], (y+1)*10.0f);
            buf->SetNormal(k.x, k.y, k.z);
            buf->SetUV0(x, y+1);
            buf->NextVert();
            }

    buf->Unlock();

    mTrackModel = new CModel(buf);
    }


void CTrack::Update( float dt )
{
	mPulse += dt;
	return;
}


#ifdef SOFTWARE_CLIPPING
int Clip(I3DVERTBUF* in, I3DVERTBUF* out)
{
	int outSize = 0;

	float camLook[3];
	// forward vector from model_view_matrix
	float fx, fy, fz;
/*	fx = model_view_matrix.z.x;
	fy = model_view_matrix.z.y;
	fz = model_view_matrix.z.z;*/
	float cx, cy, cz;
	gCamera->GetPosition(&cx, &cy, &cz);
	gCamera->GetLookAt(&camLook[0], &camLook[1], &camLook[2]);
	fx = cx - camLook[0];
	fy = cy - camLook[1];
	fz = cz - camLook[2];
	
	out->Lock(out->mNum);
	in->Lock(in->mNum);
	for (int i = 0; i < in->mNum; i += 3)
	{
		glvert_s tri[3];
		float normal[3];
		COLOR4B color;

		const glvert_s* res;
		size_t sizeClipped;
		int numClipped;

		in->GetPos(tri[0].xyz[0], tri[0].xyz[1], tri[0].xyz[2]);

		in->GetUVs(tri[0].st[0], tri[0].st[1]);
		in->GetNormal(normal[0], normal[1], normal[2]);
		in->GetColor(color);
		in->NextVert();
		for (int k = 1; k < 3; k++)
		{
			in->GetPos(tri[k].xyz[0], tri[k].xyz[1], tri[k].xyz[2]);
			in->GetUVs(tri[k].st[0], tri[k].st[1]);
			in->NextVert();
		}
		

		// check that at least one point is in front of near
		//   and at least one is in front of far
//		if(is_in_front(tri[0].xyz, tri[1].xyz, tri[2].xyz))
		{
	
			if (is_clipping_required(tri, 3))
			{
				clip(tri, 3, &res, &sizeClipped);
	//			numClipped = sizeClipped / sizeof(glvert_s);
				numClipped = sizeClipped;
				if (numClipped >= 3)
				{
					for (int t = 1; t < numClipped - 1; t++)
					{
						out->SetPos(res[0].xyz[0], res[0].xyz[1], res[0].xyz[2]);
						out->SetUV0(res[0].st[0], res[0].st[1]);
						out->SetColor(color);
						out->SetNormal((VECTOR3*)normal);
						out->NextVert();

						for (int k = t; k < t+2; k++)
						{
							out->SetPos(res[k].xyz[0], res[k].xyz[1], res[k].xyz[2]);
							out->SetUV0(res[k].st[0], res[k].st[1]);
							out->SetColor(color);
							out->SetNormal((VECTOR3*)normal);
							out->NextVert();				
						}
						outSize += 3;
					}
				}
			} else {
				for (int k = 0; k < 3; k++)
				{
					out->SetPos(tri[k].xyz[0], tri[k].xyz[1], tri[k].xyz[2]);
					out->SetUV0(tri[k].st[0], tri[k].st[1]);
					out->SetColor(color);
					out->SetNormal((VECTOR3*)normal);
					out->NextVert();
				}
				outSize += 3;
			}

		} 
	}
	in->Unlock();
	out->Unlock();

	return outSize;
}
#endif

void CTrack::Render()
{

	glColor4f(1.0f, 0.0f, 0.0f, sin(mPulse)/4.0f + 0.25f);
	blocks->Render();

	//glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    //glDepthMask(GL_FALSE);
    SVector3 dir = gCamera->GetDirection();
    dir.Normalize();

    glDepthMask(GL_FALSE);
    //glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(1.0f, 1.02f, 1.0f);
	
    glColor4f(0.0f, 0.6f, 0.0f, 1.0f);
	mTrackModel->Render();

    glDepthMask(GL_TRUE);

    //glTranslatef(dir.x*5.0f, dir.y*5.0f, dir.z*5.0f);
    //mTrackModel->Render();

    glPopMatrix();
    glPushMatrix();


    //glTranslatef(0.0f, -1.0f, 0.0f);
    //glTranslatef(-dir.x*0.2f, -dir.y*0.2f - 0.05f, -dir.z*0.2f);

	EnableLighting( &gLights, 0.0f, 0.0f, 0.0f );

	mTexture->Bind();

	glColor4f(1.0f, 0.5f, 1.0f, 1.0f);
	mTrackModel->Render();

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//glMatrixMode(GL_TEXTURE);
	//glScalef(5.0f, 5.0f, 5.0f);
	
	//glColor4f(0.0f, 0.0f, 1.0f, sin(mPulse)/4.0f + 0.25f);
	//mTrackModel->Render();

	//glLoadIdentity();
	//glMatrixMode(GL_MODELVIEW);

	mTexture->Unbind();

	//glDisable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
}


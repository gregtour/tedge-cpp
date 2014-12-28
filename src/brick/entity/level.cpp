#include <cmath>

#include "../main.h"
#include "level.h"
#include "brick.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "../../engine/graphics/buffer/I3dVertBuf.h"

const char*		LEVEL_BACKGROUND = "thin.png";

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
	CPTriangle  *mTri1, *mTri2;
	if (phys)
	{
		mTri1 = new CPTriangle(true);
		mTri2 = new CPTriangle(true);
	}
	
	SVector3 normal = ((p1-p2).CrossProduct(p1-p4));
	normal.Normalize();

	if (phys)
	{
		mTri1->SetPoint(p1, 0);
		mTri1->SetPoint(p2, 1);
		mTri1->SetPoint(p3, 2);
		mTri2->SetPoint(p1, 0);
		mTri2->SetPoint(p3, 1);
		mTri2->SetPoint(p4, 2);
	}

	AddPoint(&p1, &normal, verts);
	AddPoint(&p2, &normal, verts);
	AddPoint(&p3, &normal, verts);
	AddPoint(&p1, &normal, verts);
	AddPoint(&p3, &normal, verts);
	AddPoint(&p4, &normal, verts);

	if (phys)
	{
		mTri1->SetCollisionMask(1|2);
		mTri2->SetCollisionMask(1|2);
		phys->Add(mTri1);
		phys->Add(mTri2);
	}
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


CLevel::CLevel(const char** layout)
{
	I3DVERTBUF* verts = new I3DVERTBUF(true, I3D_VC_XYZ | I3D_VC_NORMAL | I3D_VC_UV1, (1*6*2)*3);
	verts->Lock((1*6*2)*3);
	MakeBox(SVector3(0.0f, 0.0f, -1.0f), SVector3(40.0f, 30.0f, 0.0f), verts, NULL);
	verts->Unlock();

	mModel = new CModel(verts);
	mTexture = CManagedTexture::Load(&gResourceManager, LEVEL_BACKGROUND);

	mNumBricks = 0;
	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 10; y++)
		{
			CBrick* brick;
			switch (layout[x][y])
			{
			case '*':
				brick = new CBrick(SVector3(x*4.0f, y*2.0f, 0.0f),
									SVector3(1.0f, 1.0f, 1.0f),
									NORMAL);
				break;
			case ' ':
			default:
				brick = NULL;
				break;
			}

			if (brick)
			{
				gEntities.Add(brick);
				mNumBricks++;
			}
		}
}


CLevel::~CLevel()
{
	if (mModel)
	{
		delete mModel;
		mModel = NULL;
	}
}

void CLevel::Update( float dt )
{
	mPulse += dt;
	if (mNumBricks <= 0)
	{
		ChangeGameState(NEXT_LEVEL);
	}
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

void CLevel::Render()
{
	mTexture->Bind();

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	mModel->Render();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glMatrixMode(GL_TEXTURE);
	glScalef(5.0f, 5.0f, 5.0f);
	
	glColor4f(0.0f, 0.0f, 1.0f, sin(mPulse)/4.0f + 0.25f);
	mModel->Render();

	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	mTexture->Unbind();

	glDisable(GL_BLEND);
}


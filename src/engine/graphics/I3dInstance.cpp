/*----------------------------------------------------------------------------
    I3D INSTANCE.CPP

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dModel.h"
#include    "I3dInstance.h"
#include    "I3dMixer.h"
#include    "I3dIndxBuf.h"
#include    "Material4.h"
#include    "I3dShader1.h"
#include    "I3dColor.h"
#include    "I3dRenderPrim.h"


/*----------------------------------------------------------------------------
    CONSTRUCTOR
*/

I3DINSTANCE::I3DINSTANCE(
    I3DMODEL*   model,
    int         flags)
    {
    int         i;

    SlAssert(model);
    SlAssert(model->GetSkeleton());

    mModel          = model;
    mFlags          = flags;
    mMixer          = new I3DMIXER(mModel, this);
    mInstanceVb     = new I3DVERTBUF(true, mModel->mVertType, mModel->mNumVerts);   // MMH: I don't think we need this - use a temp buf
    mNodeMatrices   = (MATRIX4*)SlMemAlloc(sizeof(MATRIX4) * mModel->mSkeleton->GetNumBones());
    for (i=0; i<mModel->mSkeleton->GetNumBones(); i++)
        {
        Mat4Identity(&mNodeMatrices[i]);
        }
    mAnimWeights    = (float*)SlMemAlloc(sizeof(float) * mModel->mSkeleton->GetNumBones());
    }


/*----------------------------------------------------------------------------
    DESTRUCTOR
*/

I3DINSTANCE::~I3DINSTANCE()
    {
    SlSafeFree(mNodeMatrices);
    SlSafeFree(mAnimWeights);
    SlSafeDelete(mInstanceVb);
    SlSafeDelete(mMixer);
    }


/*----------------------------------------------------------------------------
*/

MATRIX4* I3DINSTANCE::GetMatrixBuffer()
    {
    SlAssert(mNodeMatrices);
    return(mNodeMatrices);
    }


/*----------------------------------------------------------------------------
*/

float* I3DINSTANCE::GetEmptyWeightBuffer()
    {
    SlAssert(mAnimWeights);
    SlMemClearD(mAnimWeights, mModel->mSkeleton->GetNumBones());
    return(mAnimWeights);
    }


/*----------------------------------------------------------------------------
*/

I3DVERTBUF* I3DINSTANCE::GetVertexBuffer()
    {
    return (mInstanceVb);
    }


/*----------------------------------------------------------------------------
*/

void I3DINSTANCE::Update(
    float   dt)
    {
    int     i;

    for (i=0; i<mModel->mSkeleton->GetNumBones(); i++)
        {
        Mat4Identity(&mNodeMatrices[i]);        // MMH: do we really need to clear this?  Are we later accumulating or recalculating?
        }
    mMixer->Update(dt);
    }


/*----------------------------------------------------------------------------
*/

void I3DINSTANCE::Render()
    {
    SlAssert(mInstanceVb);

    mModel->Render(mInstanceVb);
    //mModel->Render(mInstanceVb, mNodeMatrices);
    }


/*----------------------------------------------------------------------------
    RENDER SKINNED
    Recursively render all nodes of this model using the skinned vertex buffer.
*/

void I3DINSTANCE::RenderSkinned()
    {
    SlAssert(mInstanceVb);
    SlAssert(mModel->GetRootNode());

    I3dSelectBuffers(mInstanceVb, mModel->GetIndexBuffer());
    RenderNodeSkinned(mModel->GetRootNode());
    }


/*----------------------------------------------------------------------------
    RENDER NODE SKINNED
    Recursively render nodes.  This is for verts that have been pre-transformed
    using the skeletal animation system.  Therefore, matrices are not
    set since we are using pre-transformed verts in model space.

    However, we do set materials and pay attention to other node flags.
*/

void I3DINSTANCE::RenderNodeSkinned(
    I3DNODE*    node)
    {
    I3DNODE*    child;

    // abort us and children if not supposed to draw
    if (!node->mFlags & NODE4_DRAW)
        return;

    // if we need to draw the triangles associated with this node...
    if (node->mNumTris > 0)
        {
        // set the material and shader
        I3dRenderPrimIdxNew(I3D_TRILIST, node->mNumTris, node->mStartIndex, 0, 0, mModel->mNumVerts);
        }

    // recurse into children
    child = node->mChildren;
    while (child)
        {
        RenderNodeSkinned(child);
        child = child->mNextSibling;
        }

    }


/*----------------------------------------------------------------------------
    BLEND VERTICES
    This is where the magic happens.  Transform all verts in model by skeleton.
    May want to mod this to take a subrange.
*/

void I3DINSTANCE::BlendVertices()
    {
    MATRIX4     matBlend;
    VECTOR3     final;
    VECTOR3*    posDest;
    VECTOR3*    normDest;
    int         vertSize;
    UBYTE*      ids;
    float*      weights;
    int         v;
    int         total;
    char*       vbp;

    SlAssertEx(mModel->mVertType & I3D_VC_NORMAL, "We are expecting normals!  If not, write another routine with no normals to avoid the conditional in the loop.")

    vertSize = mInstanceVb->mVertSize;
    ids      = mModel->mBoneIds;
    weights  = mModel->mBoneWeights;
    total    = mModel->mNumVerts;

    mInstanceVb->Lock();
    vbp      = mInstanceVb->GetPointer();
    posDest  = (VECTOR3*)(vbp + mInstanceVb->mOffsetPos);
    normDest = (VECTOR3*)(vbp + mInstanceVb->mOffsetNormal);

    for (v=0; v<total; v++)
        {
        matBlend = mNodeMatrices[ids[0]];

        if (ids[1])
            { 
            Mat4ScaleUniform(&matBlend, weights[0]);
            Mat4ScaleAdd(&mNodeMatrices[ids[1]], weights[1], &matBlend);
            
            if (ids[2])
                {
                Mat4ScaleAdd(&mNodeMatrices[ids[2]], weights[2], &matBlend);

                if (ids[3])
                    {
                    Mat4ScaleAdd(&mNodeMatrices[ids[3]], weights[3], &matBlend);
                    }
                }
            }

        Mat4PointTransform(&mModel->mPos[v],    &matBlend, posDest);
        Mat4PointTransform(&mModel->mNormal[v], &matBlend, normDest);
        // final.Normalize();

        ids      += 4;
        weights  += 4;
        *(int*)&posDest  += vertSize;
        *(int*)&normDest += vertSize;
        }
    mInstanceVb->Unlock();
    }
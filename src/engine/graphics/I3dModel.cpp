/*----------------------------------------------------------------------------
    I3D MODEL.CPP

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dMath.h"
#include    "I3dIndxBuf.h"
#include    "I3dModel.h"
#include    "I3dInstance.h"
#include    "I3dAnim.h"
#include    "Material4.h"
#include    "I3dShader1.h"
#include    "I3dColor.h"
#include    "I3dRenderPrim.h"
#include    "assimp.h"
#include    "aiScene.h"
#include    "aiPostProcess.h"

I3DMODEL*    I3DMODEL::gList          = NULL;

/*----------------------------------------------------------------------------
    CONSTRUCTOR
*/

I3DMODEL::I3DMODEL()
    {
    int     i;

    mEVData         = NULL;
    mRootNode       = NULL;
    mSkeleton       = NULL;
    mMaterials      = NULL;
    mNumTris        = 0;
    mNumVerts       = 0;
    mNumNodes       = 0;
    //mNumBones       = 0;
    mData           = NULL;
    mIndices        = NULL;
    mStaticInstance = NULL;

    for (i=0; i<MAX_ANIMATIONS; i++)
        mTargetedAnims[i] = NULL;
    }


/*----------------------------------------------------------------------------
    DESTRUCTOR
*/

I3DMODEL::~I3DMODEL()
    {
    SlSafeDelete(mIndices);

    if (mEVData)
        {
        SlSafeFree(mEVData);
        mRootNode        = NULL;
        mData        = NULL;
        mSkeleton    = NULL;
        mBoneIds     = NULL;
        mBoneWeights = NULL;
        mMaterials   = NULL;
        // what about animations stored in there?
        }
    else
        {
        SlSafeDeleteArray(mRootNode);
        //SlSafeFree(mRootNode);
        SlSafeFree(mData);
        //SlSafeDeleteArray(mSkeleton);
        SlSafeFree(mBoneIds);
        SlSafeFree(mBoneWeights);
        //SlSafeFree(mMaterials);
        SlSafeDeleteArray(mMaterials);
        }

    SlSafeDelete(mSkeleton);
    }



/*----------------------------------------------------------------------------
*/

void I3DMODEL::VertexSwapEndian(
    char*       data)
    {
    int         vertSize;
    unsigned int i;

    vertSize = VertexSize(mVertType);
    for (i=0; i<mNumVerts * vertSize; i += 4)
        {
        SlSwapFromLittle32(data);
        data += 4;
        }
    }


/*----------------------------------------------------------------------------
*/

void I3DMODEL::NodeIndicesToPointers(void)
    {
    for (int i = 0; i < mNumNodes; i++)
        {
        if (mRootNode[i].mParent)
            mRootNode[i].mParent = mRootNode + (unsigned int)mRootNode[i].mParent - 1;
        if (mRootNode[i].mChildren)
            mRootNode[i].mChildren = mRootNode + (unsigned int)mRootNode[i].mChildren - 1;
        if (mRootNode[i].mNextSibling)
            mRootNode[i].mNextSibling = mRootNode + (unsigned int)mRootNode[i].mNextSibling - 1;
        }
    }


/*----------------------------------------------------------------------------
*/

void I3DMODEL::NodePointersToIndices(
    char*       data)
    {
    I3DNODE* nodes = (I3DNODE*)data;
    for (int i = 0; i < mNumNodes; i++)
        {
        if (nodes[i].mParent)
            nodes[i].mParent = (I3DNODE*)(nodes[i].mParent - mRootNode + 1);
        if (nodes[i].mChildren)
            nodes[i].mChildren = (I3DNODE*)(nodes[i].mChildren - mRootNode + 1);
        if (nodes[i].mNextSibling)
            nodes[i].mNextSibling = (I3DNODE*)(nodes[i].mNextSibling - mRootNode + 1);
        }
    }


/*----------------------------------------------------------------------------
*/

void I3DMODEL::NodeSwapEndian(
    char*       data)
    {
    I3DNODE* nodes = (I3DNODE*)data;
    for (int i = 0; i < mNumNodes; i++)
        {
        float* m = (float*)&nodes[i].mMatUser;
        for (int j = 0; j < 16; j++)
            {
            SlSwapFromLittle32(m);
            m++;
            }
        SlSwapFromLittle32(&nodes[i].mBounds.max.x);
        SlSwapFromLittle32(&nodes[i].mBounds.max.y);
        SlSwapFromLittle32(&nodes[i].mBounds.max.z);
        SlSwapFromLittle32(&nodes[i].mBounds.min.x);
        SlSwapFromLittle32(&nodes[i].mBounds.min.y);
        SlSwapFromLittle32(&nodes[i].mBounds.min.z);
        SlSwapFromLittle32(&nodes[i].mSphereCenter.x);
        SlSwapFromLittle32(&nodes[i].mSphereCenter.y);
        SlSwapFromLittle32(&nodes[i].mSphereCenter.z);
        SlSwapFromLittle32(&nodes[i].mSphereRadius);
        SlSwapFromLittle32(&nodes[i].mBoneId);
        SlSwapFromLittle32(&nodes[i].mFlags);
        SlSwapFromLittle32(&nodes[i].mNumTris);
        SlSwapFromLittle32(&nodes[i].mMaterial);
        SlSwapFromLittle32(&nodes[i].mStartIndex);
        SlSwapFromLittle32(&nodes[i].mEndIndex);
        SlSwapFromLittle32(&nodes[i].mParent);
        SlSwapFromLittle32(&nodes[i].mChildren);
        SlSwapFromLittle32(&nodes[i].mNextSibling);
        }
    }

#if 0
/*----------------------------------------------------------------------------
*/

void I3DMODEL::BoneIndicesToPointers(void)
    {
    int     i;

    for (i=0; i<mNumBones; i++)
        {
        if (mSkeleton[i].mFirstChild)
            mSkeleton[i].mFirstChild = mSkeleton + (unsigned int)(mSkeleton[i].mFirstChild) - 1;
        if (mSkeleton[i].mNextSibling)
            mSkeleton[i].mNextSibling = mSkeleton + (unsigned int)(mSkeleton[i].mNextSibling) - 1;
        }
    }


/*----------------------------------------------------------------------------
*/

void I3DMODEL::BonePointersToIndices(
    char*           data)
    {
    I3DBONE*        bones;
    int             i;

    bones = (I3DBONE*)data;

    for (i=0; i<mNumBones; i++)
        {
        if (bones[i].mFirstChild)
            bones[i].mFirstChild = (I3DBONE*)(bones[i].mFirstChild - mSkeleton + 1);
        if (bones[i].mNextSibling)
            bones[i].mNextSibling = (I3DBONE*)(bones[i].mNextSibling - mSkeleton + 1);
        }
    }

/*----------------------------------------------------------------------------
*/

void I3DMODEL::BoneSwapEndian(
    char*       data)
    {
    I3DBONE*    bones;
    int         i;

    bones = (I3DBONE*)data;
    for (i=0; i<mNumBones; i++)
        {
        float* m = (float*)&bones[i].mOffsetMat;
        for (int j = 0; j < 16; j++)
            {
            SlSwapFromLittle32(m);
            m++;
            }

        SlSwapFromLittle32(&bones[i].mFirstChild);
        SlSwapFromLittle32(&bones[i].mNextSibling);
        }
    }
#endif


/*----------------------------------------------------------------------------
*/

int I3DMODEL::VertexSize(
    int          vertType)
    {
    int vertSize = 0;

    if (vertType & I3D_VC_XYZ)
        vertSize += 12;
    if (vertType & I3D_VC_NORMAL)
        vertSize += 12;
    if (vertType & I3D_VC_DIFFUSE)
        vertSize += 4;
    if (vertType & I3D_VC_UV1)
        vertSize += 8;
    if (vertType & I3D_VC_UV2)
        vertSize += 8;

    return (vertSize);
    }


/*----------------------------------------------------------------------------
*/

I3DNODE* I3DMODEL::GetNode(
    I3DNODE*        parent,
    const char*     nodeName)
    {
    if (SlStrCmp(parent->mName, nodeName) == 0)
        return (parent);
    
    I3DNODE* node = parent->mChildren;
    while (node)
        {
        I3DNODE* found = GetNode(node, nodeName);
        if (found)
            return (found);
        node = node->mNextSibling;
        }

    return (NULL);
    }


/*----------------------------------------------------------------------------
*/

I3DNODE* I3DMODEL::GetNode(
    const char*     nodeName)
    {
    return(GetNode(mRootNode, nodeName));
    }





/*----------------------------------------------------------------------------
*/

I3DINSTANCE* I3DMODEL::CreateInstance(void)
    {
    //return (NULL);
    return new I3DINSTANCE(this, 0);
    }


/*----------------------------------------------------------------------------
*/

int* I3DMODEL::RetargetAnimation(
    I3DANIM*  anim)
    {
    for (int i = 0; i < MAX_ANIMATIONS; i++)
        {
        if (!mTargetedAnims[i])
            {
            mTargetedAnims[i] = anim;
            mAnimTargets[i] = (int*)SlMemAlloc(sizeof(int) * anim->mNumAnimBones);
            for (int j = 0; j < anim->mNumAnimBones; j++)
                {
                mAnimTargets[i][j] = GetBoneId(anim->mBoneNames[j]);
                }
            return mAnimTargets[i];
            }
        if (mTargetedAnims[i] == anim)
            return mAnimTargets[i];
        }

    return (NULL);
    }


/*----------------------------------------------------------------------------
*/

void I3DMODEL::Render(
    I3DVERTBUF* buf) // = bindPose)
    {
    I3dSelectBuffers(buf, mIndices);
    I3dRenderPrimIdxNew(I3D_TRILIST, mNumTris, 0, 0, 0, mNumVerts);
    //Render(mRootNode, buf);
    }


/*----------------------------------------------------------------------------
*/

bool TestVisibility(BOUNDBOX3*  bounds)
    {
    return true;
    }


/*----------------------------------------------------------------------------
*/

void I3DMODEL::Render(
    I3DNODE*        n,      // node to render
    I3DVERTBUF*     vb)     // vertex buffer to render from
    {
    bool            visible;
    SlAssert(n);
    SlAssert(vb);

    visible = TestVisibility(&n->mBounds);
    if (visible)
        {
        if (n->mFlags & NODE4_DRAW && n->mNumTris)
            {
            if (n->mBoneId != -1)
                {
                //I3dWorldMatrix(&nodes[n->mBoneId]);
                }

            SHADER1* shader = n->mMaterial->GetShader();
            shader->Begin();
            //RenderSubset(vb, mIndices, n->mStartIndex, n->mEndIndex);
            //I3dSelectBuffers(verts, mesh->mIndices);
            //I3dRenderPrimIdxNew(I3D_TRILIST, mesh->mNumTris, 0, 0, 0, mesh->mNumVerts);
            shader->End();
            }

        //if (n->mFlags & I3D_DO_CHILDREN)
            {
            n = n->mChildren;
            while (n)
                {
                Render(n, vb);
                n = n->mNextSibling;
                }
            }
        }
    }  
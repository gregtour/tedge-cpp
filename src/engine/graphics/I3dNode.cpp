/*----------------------------------------------------------------------------
    I3D NODE.CPP

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dMath.h"
//#include    "I3dModel.h"
//#include    "Material4.h"
#include    "I3dNode.h"


/*----------------------------------------------------------------------------
    CONSTRUCTOR
*/

I3DNODE::I3DNODE()
    {
    }


/*----------------------------------------------------------------------------
    DESTRUCTOR
*/

I3DNODE::~I3DNODE()
    {
    }


/*----------------------------------------------------------------------------
*/

void I3DNODE::IndicesToPointers(
    char*   baseAddr)
    {
#if 0
        if (mRootNode[i].mParent)
            mRootNode[i].mParent = mRootNode + (unsigned int)mRootNode[i].mParent - 1;
        if (mRootNode[i].mChildren)
            mRootNode[i].mChildren = mRootNode + (unsigned int)mRootNode[i].mChildren - 1;
        if (mRootNode[i].mNextSibling)
            mRootNode[i].mNextSibling = mRootNode + (unsigned int)mRootNode[i].mNextSibling - 1;
        }
#endif
    }


/*----------------------------------------------------------------------------
*/

void I3DNODE::PointersToIndices(
    char*       baseAddr)
    {
#if 0
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
#endif
    }


/*----------------------------------------------------------------------------
*/

void I3DNODE::SwapEndian()
    {
    float*      m;
    int         i;

    m = (float*)&mMatUser;
    for (i=0; i<16; i++)
        {
        SlSwapFromLittle32(m);
        m++;
        }
    SlSwapFromLittle32(&mBounds.max.x);
    SlSwapFromLittle32(&mBounds.max.y);
    SlSwapFromLittle32(&mBounds.max.z);
    SlSwapFromLittle32(&mBounds.min.x);
    SlSwapFromLittle32(&mBounds.min.y);
    SlSwapFromLittle32(&mBounds.min.z);
    SlSwapFromLittle32(&mSphereCenter.x);
    SlSwapFromLittle32(&mSphereCenter.y);
    SlSwapFromLittle32(&mSphereCenter.z);
    SlSwapFromLittle32(&mSphereRadius);
    SlSwapFromLittle32(&mBoneId);
    SlSwapFromLittle32(&mFlags);
    SlSwapFromLittle32(&mNumTris);
    SlSwapFromLittle32(&mMaterial);
    SlSwapFromLittle32(&mStartIndex);
    SlSwapFromLittle32(&mEndIndex);
    SlSwapFromLittle32(&mParent);
    SlSwapFromLittle32(&mChildren);
    SlSwapFromLittle32(&mNextSibling);
    }
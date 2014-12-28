/*----------------------------------------------------------------------------
    I3D SKELETON.CPP

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dMath.h"
#include    "I3dSkeleton.h"
#include    "I3dBone.h"


/*----------------------------------------------------------------------------
    CONSTRUCTOR
*/

I3DSKELETON::I3DSKELETON()
    {
    mBone           = NULL;
    mNumBones       = 0;
    mLocalAlloc     = SL_FALSE;
    }


I3DSKELETON::I3DSKELETON(
    int     numBones)
    {
    mBone           = NULL;
    mNumBones       = 0;
    mLocalAlloc     = SL_FALSE;    
    CreateBones(numBones);
    }


/*----------------------------------------------------------------------------
    DESTRUCTOR
*/

I3DSKELETON::~I3DSKELETON()
    {
    if (mLocalAlloc)
        {
        SlSafeDeleteArray(mBone);
        }
    }


/*----------------------------------------------------------------------------
    CREATE BONES
    Allocate bone array.
*/

void I3DSKELETON::CreateBones(
    int         numBones)
    {
    SlAssertEx(numBones > 0, "Illegal number of bones!");
    SlAssertEx(mBone == NULL, "Bone array already exists!");
    mNumBones   = numBones;
    mBone       = new I3DBONE[mNumBones];
    mLocalAlloc = SL_TRUE;
    }


/*----------------------------------------------------------------------------
    SET BONES
    Tell skeleton to use existing array of bones.
*/

void I3DSKELETON::SetBones(
    int         numBones,
    I3DBONE*    bone)
    {
    SlAssertEx(mLocalAlloc == SL_FALSE, "Bone arrary already locally allocated!")
    SlAssertEx(numBones > 0, "Illegal number of bones!");
    SlAssertEx(bone, "Invalid bone array pointer!");

    mNumBones   = numBones;
    mBone       = bone;
    mLocalAlloc = SL_FALSE;
    }


/*----------------------------------------------------------------------------
    INDICES TO POINTERS
    Convert links from indices (as stored in files) to pointers for quick access.
*/

void I3DSKELETON::IndicesToPointers()
    {
    int    i;

    for (i=0; i<mNumBones; i++)
        {
        if (mBone[i].mFirstChild)
            mBone[i].mFirstChild = mBone + (unsigned int)(mBone[i].mFirstChild) - 1;
        if (mBone[i].mNextSibling)
            mBone[i].mNextSibling = mBone + (unsigned int)(mBone[i].mNextSibling) - 1;
        }
    }


/*----------------------------------------------------------------------------
    POINTERS TO INDICES.
    Convert links from pointers (as used at run-time) to indicies for writing
    to files, etc.
*/

void I3DSKELETON::PointersToIndices()
    {
    int             i;

    for (i=0; i<mNumBones; i++)
        {
        if (mBone[i].mFirstChild)
            mBone[i].mFirstChild = (I3DBONE*)(mBone[i].mFirstChild - mBone + 1);
        if (mBone[i].mNextSibling)
            mBone[i].mNextSibling = (I3DBONE*)(mBone[i].mNextSibling - mBone + 1);
        }
    }


/*----------------------------------------------------------------------------
    SWAP ENDIAN
    Swaps the endian-ness of the skeleton
*/

void I3DSKELETON::SwapEndian()
    {
    int     i;

    SlSwapFromLittle32(&mNumBones);
        
    for (i=0; i<mNumBones; i++)
        {
        mBone[i].SwapEndian();
        }
    }


/*----------------------------------------------------------------------------
    GET BONE
    Retrieve bone pointer from name.
*/

I3DBONE* I3DSKELETON::GetBone(
    const char* boneName)
    {
    int         i;

    for (i=0; i < mNumBones; i++)
        {
        if (SlStrCmp(boneName, mBone[i].mName) == 0)
            return &mBone[i];
        }
    return(NULL);
    }


/*----------------------------------------------------------------------------
    GET BONE
    Retrieve boneId from name.
*/

int I3DSKELETON::GetBoneId(
    const char*     boneName)
    {
    int             i;

    for (i=0; i<mNumBones; i++)
        {
        if (SlStrCmp(boneName, mBone[i].mName) == 0)
            return i;
        }

    return(-1);
    }
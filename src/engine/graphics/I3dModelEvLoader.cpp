/*----------------------------------------------------------------------------
    I3D MODEL EV LOADER.CPP
    Loads and saves to new ev model format.

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dMath.h"
#include    "I3dIndxBuf.h"
#include    "I3dModel.h"
#include    "I3dAnim.h"


/*----------------------------------------------------------------------------
    LOAD FROM EV
*/

int I3DMODEL::LoadFromEv(
    char*       fileName)
    {
#if 1
    int                 size;
    char*               p;
    bool                swap = false;

    SlAssert(fileName);
    SlAssert(fileName[0] != 0);

    // get the file into memory
    size = SlFileAllocLoad((void**)(&mEVData), fileName, SL_FALSE);
    if (size <= 0)
        {
        SlDbgMsg(SL_DBGMSG_ERROR, "(WARN) I3DMODEL::CreateFromFile(%s): File not found!\n", fileName);
        return(SL_FAIL);
        }

    // check the header n stuff
    p = mEVData;
    if (p[0] != 'e' || p[1] != 'V' || p[2] != 'M' || p[3] != '1')
        return (SL_FAIL);
    p += 4;

    unsigned int magic = *((unsigned int*)p);
    if (magic != I3D_MODEL_MAGIC_ENDIAN)
        {
        SlSwap32(&magic);

        if (magic == I3D_MODEL_MAGIC_ENDIAN)
            {
            //SlAssert(false); // should probably export correct endianness instead of swapping at load
            swap = true;
            }
        else
            {
            return (SL_FAIL);
            }
        }
    p += 4;

    unsigned int modelSection;
    modelSection = *((unsigned int*)p);
    if (swap)
        SlSwap32(&modelSection);
    p += 4;

    if (modelSection)
        {
        // nodes
        mNumNodes = *((unsigned int*)p);
        if (swap)
            SlSwap32(&mNumNodes);
        p += 4;

        mRootNode = (I3DNODE*)p;
        if (swap)
            NodeSwapEndian(p);
        NodeIndicesToPointers();
        p += sizeof(I3DNODE)*mNumNodes;

        // materials
        /*
        mNumMaterials = *((unsigned int*)p);
        if (swap)
            SlSwap32(&mNumMaterials);
        p += 4;

        // do something with materials or something

        p += sizeof(I3DMODEL_MAT)*mNumMaterials;
        */

        // bones
        int numBones = *((unsigned int*)p);
        if (swap)
            SlSwap32(&numBones);
        p += 4;

        mSkeleton = new I3DSKELETON();
        mSkeleton->SetBones(numBones, (I3DBONE*)p);
        if (swap)
            mSkeleton->SwapEndian();
        mSkeleton->IndicesToPointers();
        p += sizeof(I3DBONE)*numBones;

        // numVerts
        mNumVerts = *((unsigned int*)p);
        if (swap)
            SlSwap32(&mNumVerts);
        p += 4;
        
        // ids
        mBoneIds = (UBYTE*)p;
        p += 4 * mNumVerts;

        // weights
        mBoneWeights = (float*)p;
        if (swap)
            {
            for (unsigned int i = 0; i < mNumVerts*4; i++)
                {
                SlSwap32(p);
                p += 4;
                }
            }
        else
            p += sizeof(float)*4*mNumVerts;

        // vertType
        mVertType = *((unsigned int*)p);
        if (swap)
            SlSwap32(&mVertType);
        p += 4;

        int vertSize = VertexSize(mVertType);

        // vertex data
        SetVertexData(p);
        if (swap)
            VertexSwapEndian(p);
        p += vertSize * mNumVerts;

        // num tris
        mNumTris = *((unsigned int*)p);
        if (swap)
            SlSwap32(&mNumTris);
        p += 4;

        if (mNumTris)
            {
            mIndices = new I3DINDXBUF(false, mNumTris * 3);

            if (swap)
                {
                for (unsigned int i = 0; i < mNumTris * 3; i++)
                    {
                    SlSwap16((p + 2*i));
                    }
                }

            UWORD* indices = mIndices->Lock();
            SlMemCpy(indices, p, sizeof(UWORD) * 3 * mNumTris);
            mIndices->Unlock();
            p += sizeof(UWORD) * 3 * mNumTris;
            }

        SlAssert((mEVData + 12 + modelSection) == p);  // otherwise oops
        }

    unsigned int animSection;
    animSection = *((unsigned int*)p);
    if (swap)
        SlSwap32(&animSection);
    p += 4;

    if (animSection)
        {
        unsigned int numAnimations = *((unsigned int*)p);
        if (swap)
            SlSwap32(&numAnimations);
        p += 4;

        for (unsigned int a = 0; a < numAnimations; a++)
            {
            // these animations need to be tracked by the model to be unloaded
            // when the model is unloaded. also should be hooked into the
            // resource tracking system
            I3DANIM* anim = new I3DANIM();
            p += anim->Import(p, swap);
            anim->SetName(anim->mName);
            anim->Hook((MANAGEDRES**)&I3DANIM::gList);
            }
        }

    // check we didnt fsck this up
    SlAssert((mEVData + size) == p);
#endif
    return (SL_SUCCESS);
    }


/*----------------------------------------------------------------------------
*/

int I3DMODEL::SaveToEv(
    char*       fileName,
    int         numAnimations,
    I3DANIM**   animations,
    bool        swap)
    {
#if 1
    // eVM1
    // 0x0A0B0C0D - for endianness
    // int modelSectionSize
    //{int numNodes
    // I3DNODE[numNodes]
    // int numMaterials
    // material[numMaterials]
    // int numBones
    // I3DBONE[numBones]
    // int numVerts
    // boneIds[numVerts]
    // boneWeight[numVerts]
    // int vertType
    // vertex data
    // int numTris
    // indices[numTris*3]
    //}
    // int numAnims
    // sizeof(animSection)
    //{sizeof(anim)
    // I3DANIM}
    // ...
    // [eof]

    // Calculate file size
    unsigned int size        = 0;
    unsigned int modelSize   = 0;
    unsigned int animSize    = 0;
    unsigned int vertSize    = VertexSize(mVertType);

    modelSize += 4; // mNumNodes
    modelSize += sizeof(I3DNODE)*mNumNodes;
    /*
    modelSize += 4; // mNumMaterials
    modelSize += sizeof(I3DMODEL_MAT)*mNumMaterials;
    */
    modelSize += 4; // mNumBones
    modelSize += sizeof(I3DBONE)*mSkeleton->GetNumBones();
    modelSize += 4; // mNumVerts
    modelSize += 4*mNumVerts;
    modelSize += sizeof(float)*4*mNumVerts;
    modelSize += 4; // mVertType
    modelSize += vertSize*mNumVerts;
    modelSize += 4; // mNumTris
    modelSize += sizeof(UWORD) * 3 * mNumTris;

    if (animations)
        {
        animSize += 4;
        for (int i = 0; i < numAnimations; i++)
            {
            animSize += animations[i]->ExportSize();
            }
        }

    // [eVM1+endianness][modelSize][model][animSize][[numAnims]anims]
    size = 8 + 4 + modelSize + 4 + animSize;

    // Write data
    char* data = (char*)SlMemAlloc(size);
    char* p = data;

    // eV header
    p[0] = 'e';
    p[1] = 'V';
    p[2] = 'M';
    p[3] = '1';
    p += 4;

    // endianness
    *((unsigned int*)p) = I3D_MODEL_MAGIC_ENDIAN;
    if (swap)
        SlSwap32(p);
    p += 4;

    // model section size
    *((unsigned int*)p) = modelSize;
    if (swap)
        SlSwap32(p);
    p += 4;

    if (modelSize)
        {
        // nodes
        *((unsigned int*)p) = mNumNodes;
        if (swap)
            SlSwap32(p);
        p += 4;

        if (mNumNodes)
            {
            SlMemCpy((void*)p, (void*)mRootNode, sizeof(I3DNODE)*mNumNodes);
            NodePointersToIndices(p);
            if (swap)
                NodeSwapEndian(p);
            p += sizeof(I3DNODE)*mNumNodes;
            }
/*
        // materials
        *((unsigned int*)p) = mNumMaterials;
        p += 4;
        if (mNumMaterials)
            {
            // TODO: save materials some how
            //SlMemCpy((void*)p, (void*)?, sizeof(I3DMODELMAT)*mNumMaterials);
            p += sizeof(I3DMODELMAT)*mNumMaterials;
            }
*/

        // bones
        *((unsigned int*)p) = mSkeleton->GetNumBones();
        if (swap)
            SlSwap32(p);
        p += 4;

        if (mSkeleton->GetNumBones())
            {
            mSkeleton->PointersToIndices();
            SlMemCpy((void*)p, (void*)mSkeleton->GetRootBone(), sizeof(I3DBONE)*mSkeleton->GetNumBones());
            if (swap)
                mSkeleton->SwapEndian();//BoneSwapEndian(p);
            p += sizeof(I3DBONE)*mSkeleton->GetNumBones();
            }

        // num verts
        *((unsigned int*)p) = mNumVerts;
        if (swap)
            SlSwap32(p);
        p += 4;

        // bone ids
        SlMemCpy((void*)p, mBoneIds, 4*mNumVerts);
        p += 4*mNumVerts;

        // bone weights
        SlMemCpy((void*)p, mBoneWeights, sizeof(float)*4*mNumVerts);
        if (swap)
            {
            for (unsigned int i = 0; i < mNumVerts*16; i += 4)
                {
                SlSwap32(p);
                p += 4;
                }
            }
        else
            {
            p += sizeof(float)*4*mNumVerts;
            }

        // vert type
        *((unsigned int*)p) = mVertType;
        if (swap)
            SlSwap32(p);
        p += 4;

        // vertex data
        SlMemCpy((void*)p, mData, vertSize * mNumVerts);
        if (swap)
            VertexSwapEndian(p);
        p += vertSize * mNumVerts;

        // num tris
        *((unsigned int*)p) = mNumTris;
        if (swap)
            SlSwap32(p);
        p += 4;

        if (mNumTris)
            {
            UWORD* indices = mIndices->Lock();
            SlMemCpy(p, indices, sizeof(UWORD) * 3 * mNumTris);
            mIndices->Unlock();

            if (swap)
                {
                for (unsigned int i = 0; i < mNumTris * 3; i++)
                    {
                    SlSwap16(p);
                    p += 2;
                    }
                }
            else
                {
                p += sizeof(UWORD) * 3 * mNumTris;
                }
            }
        }

    // anim section size
    *((unsigned int*)p) = animSize;
    if (swap)
        SlSwap32(p);
    p += 4;

    if (animSize)
        {
        *((unsigned int*)p) = numAnimations;
        if (swap)
            SlSwap32(p);
        p += 4;

        for (int a = 0; a < numAnimations; a++)
            {
            p += animations[a]->Export(p, swap);
            }
        }

    // check we didnt fsck this up
    SlAssert((data + size) == p);

    // Save to file
    SL_FILE outFile = SlFileOpen(fileName, SL_WRITE);
    SlFileWrite(outFile, data, size);
    SlFileClose(outFile);

    // Free & Done
    SlSafeFree(data);
#endif
    return (SL_SUCCESS);
    }
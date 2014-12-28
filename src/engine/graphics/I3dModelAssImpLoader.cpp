/*----------------------------------------------------------------------------
    I3D MODEL ASSIMP LOADER.CPP

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dMath.h"
#include    "I3dIndxBuf.h"
#include    "I3dModel.h"
#include    "I3dAnim.h"
#include    "I3dNode.h"
#include    "Material4.h"
#include    "assimp.h"
#include    "aiScene.h"
#include    "aiPostProcess.h"

void BuildSkeleton(
    I3DMODEL*       model,
    I3DBONE*        parent, 
    aiNode*         node);


/*----------------------------------------------------------------------------
*/

int I3DMODEL::LoadFromAssImp(
    char*           fileName)
    {
    int             size;
    UBYTE*          buffer;
    int             numBones;
    unsigned int    m;
    I3DNODE*        node;
    unsigned int    i;

    SlAssert(fileName);
    SlAssert(fileName[0] != 0);

    // get the file into memory
    size = SlFileAllocLoad((void**)(&buffer), fileName, SL_FALSE);
    if (size <= 0)
        {
        SlDbgMsg(SL_DBGMSG_ERROR, "(WARN) I3DMODEL::CreateFromFile(%s): File not found!\n", fileName);
        return(SL_FAIL);
        }
    
    //const aiScene* imported = importer.ReadFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_Quality, NULL);
    //importer.ReadFile(fileName, 0);

    //const aiScene* imported = aiImportFile(fileName, 0);
    const aiScene* imported = aiImportFileFromMemory((const char*)buffer, size, 0, fileName);

    if (!imported)
        {
        SlDbgMsg(SL_DBGMSG_ERROR, "(WARN) I3DMODEL::CreateFromFile(%s): assimp failed to import file.\n", fileName);
        return(SL_FAIL);
        }

    // Find out how big the model is
    mNumVerts = 0;
    mNumTris  = 0;
    mVertType = I3D_VC_XYZ;
    numBones = 1;

    // allocate materials - do this now so nodes can point to materials
    mNumMaterials = imported->mNumMaterials;
    mMaterials   = new MATERIAL4[mNumMaterials];

    // convert materials
    for (i=0; i<mNumMaterials; i++)
        {
        //imported->mMaterials[t]->GetTexture(?,?,?)
        }

    // allocate nodes
    mNumNodes = imported->mNumMeshes+1;
    //mRootNode = (I3DNODE*)SlMemAlloc(sizeof(I3DNODE) * mNumNodes);
    mRootNode = new I3DNODE[mNumNodes];

    // convert nodes
    // MMH: do AssImp meshes have a "hierarchy" - if not, I'll just make them linear for now.
    node = mRootNode;
    SlStrCpy(node->mName, "Root");
    node->mFlags       = NODE4_DRAW | NODE4_COLLIDE;
    node->mNumTris     = 0;
    node->mMaterialId  = -1;
    node->mMaterial    = &mMaterials[node->mMaterialId];
    node->mStartIndex  = -1;
    node->mEndIndex    = -1;
    node->mParent      = NULL;
    node->mChildren    = (node+1);
    node->mNextSibling = NULL;
    node++;

    for (m=0; m<imported->mNumMeshes; m++)
        {
        aiMesh* mesh = imported->mMeshes[m];

        // sum up the vert components we need
        if (mesh->HasNormals())
            mVertType |= I3D_VC_NORMAL;
        if (mesh->HasVertexColors(0))
            mVertType |= I3D_VC_DIFFUSE;
        if (mesh->HasTextureCoords(0))
            mVertType |= I3D_VC_UV1;
        if (mesh->HasTextureCoords(1))
            mVertType |= I3D_VC_UV2;

        SlStrCpy(node->mName, mesh->mName.data);
        Mat4Identity(&node->mMatUser);
        node->mFlags       = NODE4_DRAW | NODE4_COLLIDE;
        node->mNumTris     = mesh->mNumFaces;
        node->mMaterialId  = mesh->mMaterialIndex;
        node->mMaterial    = &mMaterials[node->mMaterialId];
        node->mStartIndex  = mNumVerts;
        node->mEndIndex    = mNumVerts + mesh->mNumVertices;
        node->mParent      = mRootNode;
        node->mChildren    = NULL;
        node->mNextSibling = (node+1);

        mNumVerts += mesh->mNumVertices;
        mNumTris  += mesh->mNumFaces;
        numBones  += mesh->mNumBones;
        node++;
        }
    node--;
    node->mNextSibling = NULL;

    int vertSize = VertexSize(mVertType);

    // allocate space for it
    mIndices     = new I3DINDXBUF(false, mNumTris * 3);
    mData        = (char*)SlMemAlloc(mNumVerts * vertSize);
    mSkeleton    = new I3DSKELETON(numBones);
    mBoneIds     = (UBYTE*)SlMemAlloc(mNumVerts * 4);
    mBoneWeights = (float*)SlMemAlloc(sizeof(float) * mNumVerts * 4);

    SlMemClear(mBoneIds, mNumVerts * 4);

    SetVertexData(mData);

    // root bone
    int boneId = 0;
//    SlStrCpy(mSkeleton[boneId].mName, "root");
    mSkeleton->GetBone(boneId)->SetName("root");
    Mat4Identity(&mSkeleton->GetBone(boneId)->mOffsetMat);
    mSkeleton->GetBone(boneId)->SetNextSibling(NULL);
    mSkeleton->GetBone(boneId)->SetChild(NULL);
//    mSkeleton[boneId].SetName("root");
//    Mat4Identity(&mSkeleton[boneId].mOffsetMat);
//    mSkeleton[boneId].mNextSibling = NULL;
//    mSkeleton[boneId].mFirstChild = NULL;
    boneId++;

    // copy vertex data
    unsigned int v;
    unsigned int offs = 0;
    mIndices->Lock();
    for (unsigned int m = 0; m < imported->mNumMeshes; m++)
        {
        aiMesh* mesh = imported->mMeshes[m];

        // position
        SlMemCpy(&mPos[offs], mesh->mVertices, mesh->mNumVertices * 12);

        // copy normals
        if (mVertType & I3D_VC_NORMAL)
            {
            if (mesh->HasNormals())
                SlMemCpy(&mNormal[offs], mesh->mNormals, mesh->mNumVertices * 12);
            else
                SlMemClearB(&mNormal[offs], mesh->mNumVertices * 12);
            }

        // copy color
        if (mVertType & I3D_VC_DIFFUSE)
            {
            if (mesh->HasVertexColors(0))
                {
                for (v = 0; v < mesh->mNumVertices; v++)
                    {
                    aiColor4D* c = &mesh->mColors[0][offs+v];
                    mColor[offs+v] = PackColor4f(c->r, c->g, c->b, c->a);
                    }
                }
            else
                SlMemClearB(&mColor[offs], mesh->mNumVertices * 4);
            }

        // copy UVs
        if (mVertType & I3D_VC_UV1)
            {
            if (mesh->HasTextureCoords(0))
                {
                for (v = 0; v < mesh->mNumVertices; v++)
                    {
                    mUV0[2*(offs+v)]    = mesh->mTextureCoords[0][offs+v].x;
                    mUV0[2*(offs+v)+1]  = mesh->mTextureCoords[0][offs+v].y;
                    }
                }
            else
                SlMemClearB(&mUV0[offs], mesh->mNumVertices * 8);
            }

        if (mVertType & I3D_VC_UV2)
            {
            if (mesh->HasTextureCoords(1))
                {
                for (v = 0; v < mesh->mNumVertices; v++)
                    {                     
                    mUV1[2*(offs+v)]    = mesh->mTextureCoords[1][offs+v].x;
                    mUV1[2*(offs+v)+1]  = mesh->mTextureCoords[1][offs+v].y;
                    }
                }
            else
                SlMemClearB(&mUV1[offs], mesh->mNumVertices * 8);
            }

        // add indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
            aiFace face = mesh->mFaces[i];
            SlAssert(face.mNumIndices == 3);

            mIndices->AddTri(face.mIndices[0] + offs, face.mIndices[1] + offs, face.mIndices[2] + offs);
            }

        // articulated animation
        if (mesh->mNumBones == 1)
            mRootNode[m+1].mBoneId = boneId;
        else
            mRootNode[m+1].mBoneId = -1;

        // add bones and weights
        for (unsigned int i = 0; i < mesh->mNumBones; i++)
            {
            int newBoneId;
            aiBone* b = mesh->mBones[i];
            //SlStrCpy(mSkeleton[boneId].mName, b->mName.data);
            //mSkeleton[boneId].SetName(b->mName.data);
            mSkeleton->GetBone(boneId)->SetName(b->mName.data);

            newBoneId = boneId;
            // locate any existing bone, update bone id here
            for (int j = 0; j < boneId; j++)
                {
                //if (SlStrCmp(mSkeleton[boneId].mName, mSkeleton[j].mName) == 0)
                if (SlStrCmp(mSkeleton->GetBone(boneId)->GetName(), mSkeleton->GetBone(j)->GetName()) == 0)
                    newBoneId = j;
                }

            //SlMemCpy(&mSkeleton[boneId].mOffsetMat, &b->mOffsetMatrix, sizeof(MATRIX4));
            if (boneId == newBoneId)
                {
                //Mat4Transpose((MATRIX4*)&b->mOffsetMatrix, &mSkeleton[boneId].mOffsetMat);
                 Mat4Transpose((MATRIX4*)&b->mOffsetMatrix, mSkeleton->GetBone(boneId)->GetMatrix());
                }

            mSkeleton->GetBone(boneId)->SetNextSibling(NULL);
            mSkeleton->GetBone(boneId)->SetChild(NULL);
//            mSkeleton[boneId].mNextSibling = NULL;
//            mSkeleton[boneId].mFirstChild = NULL;

            for (unsigned int v = 0; v < b->mNumWeights; v++)
                {
                aiVertexWeight w = b->mWeights[v];

                int j = 0;
                while (mBoneIds[4*(offs+w.mVertexId) + j] && j < 4)
                    j++;
                
                if (j < 4)
                    {
                    mBoneIds[4*(offs+w.mVertexId) + j]      = newBoneId;
                    mBoneWeights[4*(offs+w.mVertexId) + j]  = w.mWeight;
                    }
                }

            if (boneId == newBoneId)
                boneId++;
            }

        offs += mesh->mNumVertices;
        }
    mIndices->Unlock();

    // MMH: this isn't correct anymore.  
    // We were previously over-allocating the number of bones we'd need because AssImp may duplicate bones?
    //mNumBones = boneId;
    // HUGE HACK ALERT
    mSkeleton->mNumBones = boneId;

    //BuildSkeleton(&mSkeleton[0], imported->mRootNode);
    BuildSkeleton(this, mSkeleton->GetRootBone(), imported->mRootNode);

    // load animations
    for (unsigned int a = 0; a < imported->mNumAnimations; a++)
        {
        aiAnimation* anim = imported->mAnimations[a];
        I3DANIM::CreateFromAI(anim);
        }

    aiReleaseImport(imported);
    SlSafeFree(buffer);
    
    return (SL_SUCCESS);
    }

/*----------------------------------------------------------------------------
*/

void I3DMODEL::SetVertexData(
    char*       data)
    {
    mData = data;

    mPos = (VECTOR3*)data;
    data += mNumVerts * 12;

    if (mVertType & I3D_VC_NORMAL)
        {
        mNormal = (VECTOR3*)data;
        data += mNumVerts * 12;
        }
    else
        mNormal = NULL;

    if (mVertType & I3D_VC_DIFFUSE)
        {
        mColor = (UDWORD*)data;
        data += mNumVerts * 4;
        }
    else
        mColor = NULL;

    if (mVertType & I3D_VC_UV1)
        {
        mUV0 = (float*)data;
        data += mNumVerts * 8;
        }
    else
        mUV0 = NULL;

    if (mVertType & I3D_VC_UV2)
        {
        mUV1 = (float*)data;
        data += mNumVerts * 8;
        }
    else
        mUV1 = NULL;
    }


/*----------------------------------------------------------------------------
*/

void BuildSkeleton(
    I3DMODEL*       model,
    I3DBONE*        parent, 
    aiNode*         node)
    {
    unsigned int    i;
    I3DBONE*        cur;

    SlAssert(model);

    cur = model->GetBone(node->mName.data);

    if (cur == parent || !cur)
        {
        for (i=0; i < node->mNumChildren; i++)
            BuildSkeleton(model, parent, node->mChildren[i]);
        }
    else
        {
        I3DBONE* last;

        if (parent->mFirstChild)
            {
            last = parent->mFirstChild;
            while (last->mNextSibling)
                last = last->mNextSibling;
            last->mNextSibling = cur;
            }
        else
            parent->mFirstChild = cur;

        for (i=0; i < node->mNumChildren; i++)
            BuildSkeleton(model, cur, node->mChildren[i]);
        }
    }

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dAnim.h"
#include    "aiAnim.h"
#include    "assimp.h"
#include    "aiScene.h"
#include    "aiPostProcess.h"

#define MAX_FRAMES      1000

float timeScratch[MAX_FRAMES];

I3DANIM*    I3DANIM::gList          = NULL;

I3DANIM::I3DANIM()
    {
    mFrames = NULL;
    }


I3DANIM::~I3DANIM()
    {

    }


/*----------------------------------------------------------------------------
    CREATE FROM ASSIMP
*/

I3DANIM* I3DANIM::CreateFromAI(
    aiAnimation*        data)
    {
    I3DANIM*        a;

    // see if it already exists and return a reference if it does
    a = (I3DANIM*)MANAGEDRES::GetReference(data->mName.data, gList);
    if (a)
        return(a);

    // create a font from the given file
    a = new I3DANIM();
    a->Create(data);

    // if the file didn't exist, or creation failed, free and abort
    if (a->mFrames == NULL)
        {
        delete a;
        return(NULL);
        }

    SlStrCpy(a->mName, data->mName.data);
    a->SetName(a->mName);
    a->Hook((MANAGEDRES**)&gList);

    return(a);
    }

/*----------------------------------------------------------------------------   
    CREATE FROM FILE
*/

I3DANIM* I3DANIM::CreateFromFile(
    char*               filePath)
    {
    int                 size;
    UBYTE*              buffer;
    char*               data;
    char                fileName[DS2_NAME_LEN];
    I3DANIM*            a;

    SlAssert(fileName);
    SlAssert(fileName[0] != 0);

    // see if it already exists and return a reference if it does
    SlExtractFileName(filePath, fileName);
    a = (I3DANIM*)MANAGEDRES::GetReference(fileName, gList);
    if (a)
        return(a);

    // get the file into memory
    size = SlFileAllocLoad((void**)(&buffer), fileName, SL_FALSE);
    if (size <= 0)
        {
        SlDbgMsg(SL_DBGMSG_ERROR, "(WARN) I3DMODEL::CreateFromFile(%s): File not found!\n", fileName);
        return(NULL);
        }

    //const aiScene* imported = aiImportFile(fileName, aiProcess_ConvertToLeftHanded);aiProcess_ConvertToLeftHanded
    const aiScene* imported = aiImportFileFromMemory((const char*)buffer, size, aiProcess_ConvertToLeftHanded, fileName);

    if (!imported)
        {
        SlDbgMsg(SL_DBGMSG_ERROR, "(WARN) I3DMODEL::CreateFromFile(%s): assimp failed to import file.\n", fileName);
        return(NULL);
        }

    // load animations
    for (unsigned int b = 0; b < imported->mNumAnimations; b++)
        {
        aiAnimation* anim = imported->mAnimations[b];

        aiReleaseImport(imported);
        SlSafeFree(buffer);

        a = I3DANIM::CreateFromAI(anim);
        }

    // if the file didn't exist, or creation failed, free and abort
    if (a->mFrames == NULL)
        {
        delete a;
        return(NULL);
        }

    a->SetName(fileName);
    a->Hook((MANAGEDRES**)&gList);

    return (NULL);
    }

/*----------------------------------------------------------------------------   
*/

void I3DANIM::Create(
    aiAnimation*    data)
    {
    mAnimDuration = (float)data->mDuration;
    mNumFrames    = (int)(mAnimDuration * 10.0f);     //MMH: I don't like this fixed frame-rate thingy
    mNumAnimBones = data->mNumChannels;

    mKeyTimes = (float*)SlMemAlloc(sizeof(float) * mNumFrames);
    for (int i = 0; i < mNumFrames; i++)
        mKeyTimes[i] = i * 0.1f;
    
    mBoneNames = (char**)SlMemAlloc((I3D_BONE_NAME_LEN+4) * mNumAnimBones);
    for (int i = 0; i < mNumAnimBones; i++)
        {
        mBoneNames[i] = (char*)&mBoneNames[mNumAnimBones + i * I3D_BONE_NAME_LEN / 4];
        }

    mFrames = (I3DBONEFRAME*)SlMemAlloc(sizeof(I3DBONEFRAME) * mNumFrames * mNumAnimBones);

    int frames = 0;
    for (int b = 0; b < mNumAnimBones; b++)
        {
        aiNodeAnim* nodeAnim = data->mChannels[b];
        SlStrCpy(mBoneNames[b], nodeAnim->mNodeName.data);

        // Position values
        if (nodeAnim->mNumPositionKeys)
            {
            int frame = 0;
            aiVectorKey cur = nodeAnim->mPositionKeys[0];
            for (unsigned int i = 1; i < nodeAnim->mNumPositionKeys; i++)
                {
                aiVectorKey last = cur;
                cur  = nodeAnim->mPositionKeys[i];

                while (mKeyTimes[frame] <= cur.mTime && frame < mNumFrames)
                    {
                    I3DBONEFRAME* f = GetBoneFrame(b, frame);
                    float alpha = (float) ((mKeyTimes[frame] - last.mTime)/(cur.mTime - last.mTime));
                    f->pos.x = alpha * cur.mValue.x + (1.0f - alpha) * last.mValue.x;
                    f->pos.y = alpha * cur.mValue.y + (1.0f - alpha) * last.mValue.y;
                    f->pos.z = alpha * cur.mValue.z + (1.0f - alpha) * last.mValue.z;
                    frame++;
                    }
                }

            while (frame < mNumFrames)
                {
                I3DBONEFRAME* f = GetBoneFrame(b, frame);
                f->pos.x = cur.mValue.x;
                f->pos.y = cur.mValue.y;
                f->pos.z = cur.mValue.z; 
                frame++;
                }
            }
        else
            {
            for (int i = 0; i < mNumFrames; i++)
                {
                I3DBONEFRAME* f = GetBoneFrame(b, i);
                f->pos.Clear();
                }
            }

        // Rotation values
        if (nodeAnim->mNumRotationKeys)
            {
            int frame = 0;
            aiQuatKey cur = nodeAnim->mRotationKeys[0];
            for (unsigned int i = 1; i < nodeAnim->mNumRotationKeys; i++)
                {
                aiQuatKey last = cur;
                cur = nodeAnim->mRotationKeys[i];

                while (mKeyTimes[frame] <= cur.mTime && frame < mNumFrames)
                    {
                    I3DBONEFRAME* f = GetBoneFrame(b, frame);
                    float alpha = (float) ((mKeyTimes[frame] - last.mTime)/(cur.mTime - last.mTime));
                    QUAT a, b;
                    a.w = -last.mValue.w;
                    a.x = last.mValue.x;
                    a.y = last.mValue.y;
                    a.z = last.mValue.z;
                    b.w = -cur.mValue.w;
                    b.x = cur.mValue.x;
                    b.y = cur.mValue.y;
                    b.z = cur.mValue.z;
                    QuatSlerp(&a, &b, alpha, &f->rot);
                    frame++;
                    }
                }

            while (frame < mNumFrames)
                {
                I3DBONEFRAME* f = GetBoneFrame(b, frame);
                f->rot.w = -cur.mValue.w;
                f->rot.x = cur.mValue.x;
                f->rot.y = cur.mValue.y;
                f->rot.z = cur.mValue.z;
                frame++;
                }
            }
        else
            {
            for (int i = 0; i < mNumFrames; i++)
                {
                I3DBONEFRAME* f = GetBoneFrame(b, i);
                QuatIdentity(&f->rot);
                }
            }

        // Scale values
        if (nodeAnim->mNumScalingKeys)
            {
            int frame = 0;
            aiVectorKey cur = nodeAnim->mScalingKeys[0];
            for (unsigned int i = 1; i < nodeAnim->mNumScalingKeys; i++)
                {
                aiVectorKey last = cur;
                cur  = nodeAnim->mScalingKeys[i];

                while (mKeyTimes[frame] <= cur.mTime && frame < mNumFrames)
                    {
                    I3DBONEFRAME* f = GetBoneFrame(b, frame);
                    float alpha = (float) ((mKeyTimes[frame] - last.mTime)/(cur.mTime - last.mTime));
                    f->scl.x = alpha * cur.mValue.x + (1.0f - alpha) * last.mValue.x;
                    f->scl.y = alpha * cur.mValue.y + (1.0f - alpha) * last.mValue.y;
                    f->scl.z = alpha * cur.mValue.z + (1.0f - alpha) * last.mValue.z;
                    frame++;
                    }
                }

            while (frame < mNumFrames)
                {
                I3DBONEFRAME* f = GetBoneFrame(b, frame);
                f->scl.Set((VECTOR3*)&cur.mValue);
                frame++;
                }
            }
        else
            {
            for (int i = 0; i < mNumFrames; i++)
                {
                I3DBONEFRAME* f = GetBoneFrame(b, i);
                f->scl.Clear();
                }
            }

        }
    }

/*----------------------------------------------------------------------------
*/

unsigned int I3DANIM::ExportSize(void)
    {
    int size = 0;
    size += sizeof(size);
    size += sizeof(mName);
    size += sizeof(mAnimDuration);
    size += sizeof(mNumFrames);
    size += sizeof(float) * mNumFrames;
    size += sizeof(mNumAnimBones);
    size += (I3D_BONE_NAME_LEN+4) * mNumAnimBones;
    size += sizeof(I3DBONEFRAME)*mNumFrames*mNumAnimBones;
    return (size);
    }


/*----------------------------------------------------------------------------
*/

unsigned int I3DANIM::Export(
    char*           data,
    bool            swap)
    {
    char* p = data;

    // size
    unsigned int size = ExportSize();
    *((unsigned int*)p) = size;   
    if (swap)
        SlSwapFromLittle32(p);
    p += 4;

    // mName
    SlMemCpy(p, mName, sizeof(mName));
    p += sizeof(mName);

    // mAnimDuration
    *((float*)p) = mAnimDuration;
    if (swap)
        SlSwapFromLittle32(p);
    p += 4;

    // mNumFrames
    *((int*)p) = mNumFrames;
    if (swap)
        SlSwapFromLittle32(p);
    p += 4;

    // mKeyTimes
    SlMemCpy(p, mKeyTimes, sizeof(float) * mNumFrames);
    if (swap)
        {
        for (int i = 0; i < mNumFrames; i++)
            {
            SlSwapFromLittle32(p);
            p += 4;
            }
        }
    else
        {
        p += sizeof(float) * mNumFrames;
        }

    // mNumAnimBones
    *((int*)p) = mNumAnimBones;
    if (swap)
        SlSwapFromLittle32(p);
    p += 4;

    // mBoneNames
    SlMemCpy(p, mBoneNames, (I3D_BONE_NAME_LEN+4) * mNumAnimBones);
    p += (I3D_BONE_NAME_LEN+4) * mNumAnimBones;

    // mFrames
    SlMemCpy(p, mFrames, sizeof(I3DBONEFRAME)*mNumFrames*mNumAnimBones);
    if (swap)
        {
        for (unsigned int i = 0; i < sizeof(I3DBONEFRAME)*mNumFrames*mNumAnimBones; i += 4)
            {
            SlSwapFromLittle32(p);
            p += 4;
            }
        }
    else
        {
        p += sizeof(I3DBONEFRAME)*mNumFrames*mNumAnimBones;
        }

    // done
    SlAssert(data + size == p);
    return (size);
    }


/*----------------------------------------------------------------------------
*/

unsigned int I3DANIM::Import(
    char*           data,
    bool            swap)
    {
    char* p = data;

    // size
    unsigned int size = *((unsigned int*)p);
    if (swap)
        SlSwapFromLittle32(&size);
    p += 4;

    // mName
    SlMemCpy(mName, p, sizeof(mName));
    p += sizeof(mName);

    // mAnimDuration
    mAnimDuration = *((float*)p);
    if (swap)
        SlSwapFromLittle32(&mAnimDuration);
    p += 4;

    // mNumFrames
    mNumFrames = *((int*)p);
    if (swap)
        SlSwapFromLittle32(&mNumFrames);
    p += 4;

    // mKeyTimes
    mKeyTimes = (float*)p;
    if (swap)
        for (int i = 0; i < mNumFrames; i++)
            SlSwapFromLittle32(&mKeyTimes[i]);
    p += sizeof(float) * mNumFrames;

    // mNumAnimBones
    mNumAnimBones = *((int*)p);
    if (swap)
        SlSwapFromLittle32(&mNumAnimBones);
    p += 4;

    // mBoneNames
    mBoneNames = (char**)p;
    for (int i = 0; i < mNumAnimBones; i++)
        {
        mBoneNames[i] = (char*)&mBoneNames[mNumAnimBones + i * I3D_BONE_NAME_LEN / 4];
        }
    p += (I3D_BONE_NAME_LEN+4) * mNumAnimBones;

    // mFrames
    mFrames = (I3DBONEFRAME*)p;
    if (swap)
        {
        for (unsigned int i = 0; i < sizeof(I3DBONEFRAME)*mNumFrames*mNumAnimBones; i += 4)
            {
            SlSwapFromLittle32(p);
            p += 4;
            }
        }
    else
        {
        p += sizeof(I3DBONEFRAME)*mNumFrames*mNumAnimBones;
        }

    // done
    SlAssert(data + size == p);
    return (size);
    }

/*----------------------------------------------------------------------------
*/

I3DBONEFRAME* I3DANIM::GetBoneFrame(
    int     bone, 
    int     frame)
    {
    SlAssert(bone < mNumAnimBones && frame < mNumFrames);
    return (&mFrames[frame*mNumAnimBones + bone]);
    }


/*----------------------------------------------------------------------------
*/

float I3DANIM::GetBlendFactor(
    int     frameA, 
    int     frameB, 
    float   t)
    {
    float delta = mKeyTimes[frameB] - mKeyTimes[frameA];
    if (delta < 0.0f)
        delta += mAnimDuration;
        
    return (t - mKeyTimes[frameA])/delta;
    }

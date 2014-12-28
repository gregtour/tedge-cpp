/*----------------------------------------------------------------------------
    I3D MIXER.CPP

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dMixer.h"
#include    "I3dAnim.h"
#include    "I3dModel.h"
#include    "I3dInstance.h"

/*----------------------------------------------------------------------------
    FRAME TO MAT4
    Copied from QuatToMat4
    Convert a quaternion to the corresponding (row major) 4X4 matrix.
    NOTE:  s= 2.0 / ||q|| but since ||q|| = 1, then s = 2.0
*/

MATRIX4* FrameToMat4(
    I3DBONEFRAME*   f,
    MATRIX4*        m)
    {
    QUAT*           q = &f->rot;
    VECTOR3*        p = &f->pos;
    VECTOR3*        s = &f->scl;

    float           x2;
    float           y2;
    float           z2;

    float           wx2;
    float           wy2;
    float           wz2;

    float           xx2;
    float           xy2;
    float           xz2;

    float           yy2;
    float           yz2;

    float           zz2;

    // Precalcs to avoid duplication
    x2  = q->x * 2.0f;
    y2  = q->y * 2.0f;
    z2  = q->z * 2.0f;

    wx2 = q->w * x2;
    wy2 = q->w * y2;
    wz2 = q->w * z2;

    xx2 = q->x * x2;
    xy2 = q->x * y2;
    xz2 = q->x * z2;

    yy2 = q->y * y2;
    yz2 = q->y * z2;

    zz2 = q->z * z2;

    // Set the matrix
    m->e[0][0] = (1.0f - (yy2 + zz2)) * s->x;
    m->e[1][0] =         (xy2 + wz2)  * s->x;
    m->e[2][0] =         (xz2 - wy2)  * s->x;
    m->e[3][0] = p->x;
            
    m->e[0][1] =         (xy2 - wz2)  * s->y;
    m->e[1][1] = (1.0f - (xx2 + zz2)) * s->y;
    m->e[2][1] =         (yz2 + wx2)  * s->y;
    m->e[3][1] = p->y;
            
    m->e[0][2] =         (xz2 + wy2)  * s->z;
    m->e[1][2] =         (yz2 - wx2)  * s->z;
    m->e[2][2] = (1.0f - (xx2 + yy2)) * s->z;
    m->e[3][2] = p->z;
            
    m->e[0][3] = 0.0f;
    m->e[1][3] = 0.0f;
    m->e[2][3] = 0.0f;
    m->e[3][3] = 1.0f;

    return(m);
    }



/*----------------------------------------------------------------------------
*/

I3DMIXER::I3DMIXER(
    I3DMODEL*       model, 
    I3DINSTANCE*    instance)
    {
    mModel = model;
    mInstance = instance;
    Stop();
    }

/*struct ANIMTRACK
    {
    I3DANIM*        mAnim;
    I3D_PLAY_MODE   mPlayMode;
    float           mTime;
    int             mFrame;
    float           mFadeIn;
    float           mFadeOut;
    float           mTimeScale;
    };*/


/*----------------------------------------------------------------------------
*/

int I3DMIXER::Play(
    I3DANIM*        anim, 
    I3D_PLAY_MODE   mode,
    float           weight,
    float           fadeIn, 
    float           fadeOut,
    float           seek)
    {
    int             i;

    for (i=0; i<MAX_TRACKS; i++)
        {
        if (mAnimations[i].mAnim == NULL)
            {
            mAnimations[i].mAnim        = anim;
            mAnimations[i].mWeight      = weight;
            mAnimations[i].mFrame       = 0;
            mAnimations[i].mPlayMode    = mode;
            mAnimations[i].mTime        = seek;
            mAnimations[i].mFadeIn      = fadeIn;
            mAnimations[i].mFadeOut     = fadeOut;
            mAnimations[i].mTimeScale   = 1.0f;
            break;
            }
        }

    SlAssertEx(i < MAX_TRACKS, "Not enough animation playback tracks.");
    if (i == MAX_TRACKS)
        {
        return(SL_FAIL);
        }

    return(SL_SUCCESS);
    }


/*----------------------------------------------------------------------------
*/

void I3DMIXER::Clear(
    I3DANIM*    anim)
    {
    int         i;

    for (i=0; i<MAX_TRACKS; i++)
        {
        if (mAnimations[i].mAnim == anim)
            mAnimations[i].mAnim = NULL;
        }
    }


/*----------------------------------------------------------------------------
*/

void I3DMIXER::Stop()
    {
    int         i;

    for (i=0; i<MAX_TRACKS; i++)
        {
        mAnimations[i].mAnim = NULL;
        }
    }


/*----------------------------------------------------------------------------
*/

void I3DMIXER::Update(
    float       dt)
    {
    float       animationWeight;
    MATRIX4*    matrices;
    float*      weights;
    int         i;

    dt *= 10.0f;

    // process animation tracks
    matrices = mInstance->GetMatrixBuffer();
    weights  = mInstance->GetEmptyWeightBuffer();

    // process all playing animations
    for (i=0; i<MAX_TRACKS; i++)
        {
        if (mAnimations[i].mAnim)
            {
            I3DANIM* anim = mAnimations[i].mAnim;
            SlAssert(anim);

            // advance animation
            mAnimations[i].mTime += mAnimations[i].mTimeScale * dt;

            if (mAnimations[i].mTime > anim->mAnimDuration)
                {
                if (mAnimations[i].mPlayMode == I3D_LOOP)
                    {
                    mAnimations[i].mTime -= anim->mAnimDuration;
                    mAnimations[i].mFrame = 0;
                    }
                else
                    {
                    mAnimations[i].mAnim = NULL;
                    continue;
                    }
                }

            // calculate weighting 
            if (mAnimations[i].mTime < mAnimations[i].mFadeIn)
                animationWeight = mAnimations[i].mTime/mAnimations[i].mFadeIn;
            else if (mAnimations[i].mTime > mAnimations[i].mAnim->mAnimDuration - mAnimations[i].mFadeOut)
                animationWeight = (mAnimations[i].mAnim->mAnimDuration - mAnimations[i].mTime) / mAnimations[i].mFadeOut;
            else
                animationWeight = 1.0f;

            animationWeight *= mAnimations[i].mWeight;

            // find animation frames
            while (mAnimations[i].mFrame < anim->mNumFrames - 1)
                {
                if (mAnimations[i].mTime < anim->mKeyTimes[mAnimations[i].mFrame + 1])
                    break;
                mAnimations[i].mFrame++;
                }

            int nextFrame = (mAnimations[i].mFrame + 1) % anim->mNumFrames;
            float alpha = anim->GetBlendFactor(mAnimations[i].mFrame, nextFrame, mAnimations[i].mTime);


            I3DBONEFRAME blended;
            MATRIX4      local;
            // process all animated joints
            for (int j = 0; j < anim->mNumAnimBones; j++)
                {
                // blend each joint in the animation between the two frames
                I3DBONEFRAME* last = anim->GetBoneFrame(j, mAnimations[i].mFrame);
                I3DBONEFRAME* next = anim->GetBoneFrame(j, nextFrame);

                blended.pos = last->pos + (next->pos - last->pos) * alpha;
                QuatSlerp(&last->rot, &next->rot, alpha, &blended.rot);
                blended.scl = last->scl + (next->scl - last->scl) * alpha;

                // create a local transform matrix
                FrameToMat4(&blended, &local);

                // linearly blend with other animations
                int* indices = mModel->RetargetAnimation(mAnimations[i].mAnim);
                int index = indices[j];

                // be sure the mapping is in bounds
                if (index >= 0 && index < mModel->mSkeleton->GetNumBones())
                    {
                    float total = weights[index] + animationWeight;
                    float beta = animationWeight / total;

                    Mat4Lerp(&local, &matrices[index], beta, &matrices[index]);

                    weights[index] = total;
                    }
                }
            }
        }

    // create world transforms
    I3DBONE* parent = mModel->GetRootBone();
    TransformRecursive(parent, matrices, weights, NULL);
    }


/*----------------------------------------------------------------------------
*/

void I3DMIXER::TransformRecursive(
    I3DBONE*    bone,
    MATRIX4*    matrices,
    float*      weights,
    MATRIX4*    globalTransform)
    {
    MATRIX4     temp;

    int boneId = mModel->GetBoneId(bone);
    //SlMemCpy(&temp, &matrices[boneId], sizeof(MATRIX4));
    Mat4Copy(&temp, &matrices[boneId]);

    if (globalTransform)
        {
        Mat4Mul( &matrices[boneId],globalTransform, &temp);

        if (weights[boneId] > 0.1f)
            {
            Mat4Mul(&bone->mOffsetMat, &temp, &matrices[boneId]);
            }
        else
            {
            matrices[boneId] = temp;
            }
        }

    globalTransform = &temp;//&matrices[boneId];
    bone = bone->mFirstChild;
    while (bone)
        {
        TransformRecursive(bone, matrices, weights, globalTransform);
        bone = bone->mNextSibling;
        }
    }
/*----------------------------------------------------------------------------
    I3D SKELETON.CPP
*/


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DSKELETON_H
#define _I3DSKELETON_H


/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include "I3dBone.h"

/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

#define I3D_SKEL_NAME_LEN   32


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/


class I3DSKELETON
    {
    public:
                    I3DSKELETON        ();
                    I3DSKELETON        (int         numBones);
                    ~I3DSKELETON       ();

    I3DBONE*        GetBone            (const char* boneName);
    I3DBONE*        GetBone            (int         id);
    int             GetBoneId          (const char* boneName);
    int             GetBoneId          (I3DBONE*    bone);
    I3DBONE*        GetRootBone        (void);
    void            CreateBones        (int         numBones);
    void            SetBones           (int         numBones,
                                        I3DBONE*    bone);
    int             GetNumBones        (void);
    void            IndicesToPointers  (void);
    void            PointersToIndices  (void);
    void            SwapEndian         (void);

    int             mNumBones;
    private:    

    char            mName[I3D_SKEL_NAME_LEN];
    I3DBONE*        mBone;                      // pointer to bones (in index order)
    bool            mLocalAlloc;
    };


/*----------------------------------------------------------------------------
*/

inline I3DBONE* I3DSKELETON::GetRootBone()
    {
    return(mBone);
    }

/*----------------------------------------------------------------------------
*/

inline I3DBONE* I3DSKELETON::GetBone(int id)
    {
    SlAssert(id >= 0);
    SlAssert(id < mNumBones);
    return(&mBone[id]);
    }

/*----------------------------------------------------------------------------
*/

inline int I3DSKELETON::GetBoneId(I3DBONE* bone)
    {
    //  Retrieve boneId from pointer.    
    SlAssert(bone);
    SlAssert(mBone);
    if (bone)
        return(int)(bone - mBone);
    else
        return(-1);
    }

/*----------------------------------------------------------------------------
*/

inline int I3DSKELETON::GetNumBones()
    {
    return(mNumBones);
    }


/*--------------------------------------------------------------------------*/
/*                            Public Prototypes                             */    
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                             Global Variables                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#endif // _I3DSKELETON_H

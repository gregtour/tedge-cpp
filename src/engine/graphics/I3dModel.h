/*----------------------------------------------------------------------------
    I3D MODEL.H
*/


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DMODEL_H
#define _I3DMODEL_H


/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include    "ManagedResource.h"
#include    "I3dVertBuf.h"
#include    "FltMatrix4.h"
#include    "I3dBone.h"
#include    "I3dNode.h"
#include    "I3dSkeleton.h"


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

#define I3D_MODEL_MAGIC_ENDIAN      0x0A0B0C0D

#define MAX_ANIMATIONS      16


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

class I3DANIM;
class I3DINSTANCE;
class MATERIAL4;
class I3DNODE;
class I3DMODEL : public MANAGEDRES
    {
    public:

    static I3DMODEL*    gList;
    static I3DMODEL*    CreateFromFile         (char*       filePath);
    static I3DMODEL*    Find                   (char*       name);

                        I3DMODEL               ();
                        ~I3DMODEL              ();

    int                 LoadFromAssImp         (char*       fileName);
    int                 LoadAnim               (const char* animFile);
    int                 Compile                (void);

    int                 LoadFromEv             (char*       fileName);
    int                 SaveToEv               (char*       fileName,
                                                int         numAnimations,
                                                I3DANIM**   animations,
                                                bool        swapEndian = false);

    I3DINSTANCE*        CreateInstance         (void);

    I3DNODE*            GetNode                (const char* nodeName);
    I3DNODE*            GetNode                (int         id);
    I3DNODE*            GetNode                (I3DNODE*    parent,
                                                const char* nodeName);
    I3DNODE*            GetRootNode            (void);

    // proxies for skeleton
    I3DSKELETON*        GetSkeleton            (void);
    I3DBONE*            GetBone                (const char* boneName);
    I3DBONE*            GetBone                (int         id);
    int                 GetBoneId              (const char* boneName);
    int                 GetBoneId              (I3DBONE*    bone);
    I3DBONE*            GetRootBone            (void);

    I3DINDXBUF*         GetIndexBuffer         (void);

    void                Render                 (I3DVERTBUF* buf);// = bindPose);
    void                Render                 (I3DNODE*    n, 
                                                I3DVERTBUF* vb);

    int*                RetargetAnimation      (I3DANIM*    anim);
    void                SetSkeleton            (I3DSKELETON* skeleton);

    protected:

    static int          VertexSize             (int         vertType);

    void                SetVertexData          (char*       data);
    void                VertexSwapEndian       (char*       data);

    void                NodeIndicesToPointers  (void);
    void                NodePointersToIndices  (char*       data);
    void                NodeSwapEndian         (char*       data);

    void                BoneIndicesToPointers  (void);
    void                BonePointersToIndices  (char*       data);
    void                BoneSwapEndian         (char*       data);

    char*               mEVData;

    I3DINDXBUF*         mIndices;
    unsigned int        mNumTris;

    // node heirarchy
    int                 mNumNodes;
    I3DNODE*            mRootNode;

    // animation targeting
    I3DANIM*            mTargetedAnims[MAX_ANIMATIONS];
    int*                mAnimTargets[MAX_ANIMATIONS];

    public:

    I3DSKELETON*        mSkeleton;

    // vertex data    
    char*               mData;
    unsigned int        mNumVerts;
    UDWORD              mVertType;

    VECTOR3*            mPos;
    UDWORD*             mColor;
    VECTOR3*            mNormal;
    float*              mUV0;
    float*              mUV1;

    // vertex - bone weighting
    UBYTE*              mBoneIds;
    float*              mBoneWeights;

    // instance the static model for static rendering?
    I3DINSTANCE*        mStaticInstance;
    
    protected:

    // materials
    unsigned int        mNumMaterials;
    MATERIAL4*          mMaterials;
    };


/*----------------------------------------------------------------------------
    CREATE FROM FILE
*/

inline I3DMODEL* I3DMODEL::CreateFromFile(
    char*           filePath)
    {
    char            fileName[DS2_NAME_LEN];
    I3DMODEL*       m;
    char*           ext;

    // see if it already exists and return a reference if it does
    SlExtractFileName(filePath, fileName);
    ext = SlFindExtension(filePath);

    m = (I3DMODEL*)MANAGEDRES::GetReference(fileName, gList);
    if (m)
        return(m);

    // create a model from the given file
    m = new I3DMODEL();
    if (strcmp(ext, ".mdl") == 0)
        m->LoadFromEv(filePath);
    else
        m->LoadFromAssImp(filePath);

    // if the file didn't exist, or creation failed, free and abort
    if (m->mData == NULL)
        {
        delete m;
        return(NULL);
        }

    m->SetName(fileName);
    m->Hook((MANAGEDRES**)&gList);

    return(m);
    }


/*----------------------------------------------------------------------------
    FIND
*/

inline I3DMODEL* I3DMODEL::Find(
    char*   name)
    {
    return( (I3DMODEL*)FindByName(name, (MANAGEDRES*)gList) );
    }

/*----------------------------------------------------------------------------
*/

inline void I3DMODEL::SetSkeleton(I3DSKELETON* skeleton)
    {
    mSkeleton = skeleton;
    }

/*----------------------------------------------------------------------------
*/

inline I3DNODE* I3DMODEL::GetRootNode(void)
    {
    return (mRootNode);
    }

/*----------------------------------------------------------------------------
*/

inline I3DNODE* I3DMODEL::GetNode(int id)
    {
    SlAssertEx(id >= 0, "Illegal node id.");
    SlAssertEx(id < mNumNodes, "Illegal node id.");
    return (&mRootNode[id]);
    }

/*----------------------------------------------------------------------------
*/

inline I3DSKELETON* I3DMODEL::GetSkeleton()
    {
    return(mSkeleton);
    }

/*----------------------------------------------------------------------------
*/

inline I3DBONE* I3DMODEL::GetBone(const char* boneName)
    {
    SlAssert(mSkeleton);
    return(mSkeleton->GetBone(boneName));
    }

/*----------------------------------------------------------------------------
*/

inline I3DBONE* I3DMODEL::GetBone(int id)
    {
    SlAssert(mSkeleton);
    return(mSkeleton->GetBone(id));
    }

/*----------------------------------------------------------------------------
*/

inline int I3DMODEL::GetBoneId(const char* boneName)
    {
    SlAssert(mSkeleton);
    return(mSkeleton->GetBoneId(boneName));
    }

/*----------------------------------------------------------------------------
*/

inline int I3DMODEL::GetBoneId(I3DBONE*    bone)
    {
    SlAssert(mSkeleton);
    return(mSkeleton->GetBoneId(bone));
    }

/*----------------------------------------------------------------------------
*/

inline I3DBONE* I3DMODEL::GetRootBone(void)
    {
    SlAssert(mSkeleton);
    return(mSkeleton->GetRootBone());
    }

/*----------------------------------------------------------------------------
*/

inline I3DINDXBUF* I3DMODEL::GetIndexBuffer()
    {
    return(mIndices);
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

#endif // _I3DMODEL_H

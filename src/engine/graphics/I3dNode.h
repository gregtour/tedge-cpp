/*----------------------------------------------------------------------------
    I3D NODE.H

*/


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DNODE_H
#define _I3DNODE_H


/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include    "FltMatrix4.h"
//#include    "I3dBone.h"
//#include    "I3dSkeleton.h"


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/


#define I3D_NODE_NAME_LEN   32


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

//#define I3D_RENDER          0x1
//#define I3D_DO_CHILDREN     0x2

enum NODE4_FLAGS   
    {
    NODE4_DRAW     = 0x0001,
    NODE4_COLLIDE  = 0x0002
    };

class MATERIAL4;
class I3DNODE
    {
    public: 

                I3DNODE();
                ~I3DNODE();
    char        mName[I3D_NODE_NAME_LEN];

    BOUNDBOX3   mBounds;
    VECTOR3     mSphereCenter;
    float       mSphereRadius;

    int         mBoneId;
    MATRIX4     mMatUser;

    int         mFlags;
    int         mNumTris;           // number of primitives to render
    int         mMaterialId;        // index of material in this model to use
    MATERIAL4*  mMaterial;          // pointer to the material for faster access
    int         mStartIndex;        // where in index buffer to start reading
    int         mEndIndex;

    I3DNODE*    mParent;
    I3DNODE*    mChildren;
    I3DNODE*    mNextSibling;

    void        SwapEndian         (void);
    void        IndicesToPointers  (char*   baseAddr);
    void        PointersToIndices  (char*   baseAddr);
    };


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

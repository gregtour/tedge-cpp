/*----------------------------------------------------------------------------
    I3D INSTANCE.H

    4/19/2011
*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DINSTANCE_H
#define _I3DINSTANCE_H

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include    "I3dMath.h"


// I3DINSTANCE Flags
//#define I3D_INST_STORE_VB     0x0001
//#define I3D_INST_ANIMATED
//#define

/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

class I3DVERTBUF;
class I3DMODEL;
class I3DMIXER;
class I3DINSTANCE
    {
    public:
                I3DINSTANCE            (I3DMODEL*   model,
                                        int         flags);
                ~I3DINSTANCE           (void);

    void        Update                 (float       dt);
    void        BlendVertices          (void);
    void        Render                 (void);
    void        RenderSkinned          (void);
    void        RenderNodeSkinned      (I3DNODE*    node);

    MATRIX4*    GetMatrixBuffer        (void);
    float*      GetEmptyWeightBuffer   (void);
    I3DVERTBUF* GetVertexBuffer        (void);

    //static I3DVERTBUF* gScratchBuffer;
    int         mFlags;
    MATRIX4*    mNodeMatrices;
    float*      mAnimWeights;
    I3DVERTBUF* mInstanceVb;
    I3DMODEL*   mModel;
    I3DMIXER*   mMixer;
    };

#endif // _I3DINSTANCE_H

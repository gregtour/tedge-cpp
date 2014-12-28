/*----------------------------------------------------------------------------
    I3D ANIM.H
    
    4/18/2011
*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DANIM_H
#define _I3DANIM_H


/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include    "ManagedResource.h"
#include    "I3dMath.h"
#include    "I3dModel.h"

/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

#define I3D_ANIM_NAME_LEN   32

/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/


struct I3DBONEFRAME
    {
    VECTOR3     pos; 
    QUAT        rot; 
    VECTOR3     scl;
    };

struct aiAnimation;
class I3DANIM : public MANAGEDRES
    {
    public:

    static I3DANIM*     gList;
    static I3DANIM*     CreateFromFile (char*           filePath);
    static I3DANIM*     CreateFromAI   (aiAnimation*    data);
    static I3DANIM*     Find           (char*           name);

                        I3DANIM        (void);
                        ~I3DANIM       (void);
    

    void                Create         (aiAnimation*    data);

    unsigned int        ExportSize     (void);
    unsigned int        Export         (char*           data,
                                        bool            swapEndian);
    unsigned int        Import         (char*           data,
                                        bool            swapEndian);

    I3DBONEFRAME*       GetBoneFrame   (int             bone, 
                                        int             frame);

    float               GetBlendFactor (int             frameA, 
                                        int             frameB, 
                                        float           t);

    char            mName[I3D_ANIM_NAME_LEN];
    float           mAnimDuration;
    int             mNumFrames;
    float*          mKeyTimes;      // [mNumFrames]
    int             mNumAnimBones;
    char**          mBoneNames;     // [mNumAnimBones]

    private:

    I3DBONEFRAME*   mFrames;        // [mNumFrames][mNumAnimBones];
    };


/*----------------------------------------------------------------------------
    FIND
*/

inline I3DANIM* I3DANIM::Find(
    char*   name)
    {
    return( (I3DANIM*)FindByName(name, (MANAGEDRES*)gList) );
    }



#endif // _I3DANIM_H
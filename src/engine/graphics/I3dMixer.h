/*----------------------------------------------------------------------------
    I3D MIXER.H
    
    4/18/2011
*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DMIXER_H
#define _I3DMIXER_H


/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include "I3dMath.h"

/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

#define     MAX_TRACKS      4

enum I3D_PLAY_MODE
    {
    I3D_ONCE = 1,
    I3D_LOOP
    };

/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

class I3DANIM;
struct I3DANIMTRACK
    {
    I3DANIM*        mAnim;
    float           mWeight;
    I3D_PLAY_MODE   mPlayMode;
    float           mTime;
    int             mFrame;
    float           mFadeIn;
    float           mFadeOut;
    float           mTimeScale;
    };

class I3DBONE;
class I3DMODEL;
class I3DINSTANCE;
class I3DMIXER
    {
    public:
                I3DMIXER           (I3DMODEL*       model, 
                                    I3DINSTANCE*    instance);

    int         Play               (I3DANIM*        anim, 
                                    I3D_PLAY_MODE   mode = I3D_ONCE,
                                    float           weight = 1.0f,
                                    float           fadeIn = 0.0f, 
                                    float           fadeOut = 0.0f,
                                    float           seek = 0.0f);

    void        Clear              (I3DANIM*        anim);

    void        Stop               (void);

    void        TransformRecursive (I3DBONE*        node,
                                    MATRIX4*        matrices,
                                    float*          weights,
                                    MATRIX4*        globalTransform);

    void        Update             (float           dt);

    I3DMODEL*       mModel;
    I3DINSTANCE*    mInstance;

    int             mNumAnimations;
    I3DANIMTRACK    mAnimations[MAX_TRACKS];
    };




#endif // _I3DMIXER_H

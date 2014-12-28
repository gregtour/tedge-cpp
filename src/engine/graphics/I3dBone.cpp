/*----------------------------------------------------------------------------
    I3D BONE.CPP

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

#include    "SysLib.h"              // Matt's operating system library
#include    "I3dMath.h"
#include    "I3dBone.h"


/*----------------------------------------------------------------------------
    CONSTRUCTOR
*/

I3DBONE::I3DBONE()
    {
    }


/*----------------------------------------------------------------------------
    DESTRUCTOR
*/

I3DBONE::~I3DBONE()
    {
    }


/*----------------------------------------------------------------------------
    SWAP ENDIAN
    Swaps the endian-ness of a bone.
*/

void I3DBONE::SwapEndian()
    {
    int             i;
    float*          e;      //matrix element to swap

    e = (float*)&mOffsetMat;
    for (i=0; i<16; i++)
        {
        SlSwapFromLittle32(e);
        e++;
        }

    SlSwapFromLittle32(mFirstChild);
    SlSwapFromLittle32(mNextSibling);
    }
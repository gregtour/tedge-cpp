/*----------------------------------------------------------------------------
    I3D SKELETON.H

    Copyright (c) 2011 eV Interactive, LLC
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DBONE_H
#define _I3DBONE_H


/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

#define I3D_BONE_NAME_LEN   32


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

class I3DBONE
    {
    public:                       

    char        mName[I3D_BONE_NAME_LEN];
    MATRIX4     mOffsetMat;
    I3DBONE*    mFirstChild;
    I3DBONE*    mNextSibling;

                I3DBONE        ();
                ~I3DBONE       ();
    void        SwapEndian     (void);
    void        SetName        (const char* name);
    const char* GetName        (void);
    I3DBONE*    GetChild       (void);
    I3DBONE*    GetNextSibling (void);
    void        SetChild       (I3DBONE*    child);
    void        SetNextSibling (I3DBONE*    sib);
    MATRIX4*    GetMatrix      (void);
    };


inline void I3DBONE::SetName(const char* name)
    {
    SlStrNCpy(mName, name, I3D_BONE_NAME_LEN);
    }

inline const char* I3DBONE::GetName()
    {
    return(mName);
    }

inline I3DBONE* I3DBONE::GetChild()
    {
    return(mFirstChild);
    }

inline I3DBONE* I3DBONE::GetNextSibling()
    {
    return(mNextSibling);
    }

inline void I3DBONE::SetChild(I3DBONE* child)
    {
    mFirstChild = child;
    }

inline void I3DBONE::SetNextSibling(I3DBONE* sib)
    {
    mNextSibling = sib;
    }

inline MATRIX4* I3DBONE::GetMatrix()
    {
    return(&mOffsetMat);
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

#endif  // _I3DBONE_H

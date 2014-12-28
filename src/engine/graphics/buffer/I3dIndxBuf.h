/*----------------------------------------------------------------------------
    I3D INDX BUF

    MMH 2007/03/22: Created

    Copyright (c) 2007 Matthew Harmon
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DINDXBUF_H
#define _I3DINDXBUF_H



// ************* TEST STUFF ****
// REMOVE ME
#ifndef _SIMPLE_STUFF
#define _SIMPLE_STUFF

#include <string.h>

#define UDWORD	unsigned long
#define UWORD	unsigned short

struct VECTOR3 { float x; float y; float z; };
void V3Copy( VECTOR3* dest, VECTOR3* src );

#define		SlAssert(ignore)	(void*)0
#define		SL_TRUE		true
#define		SL_FALSE	false
#define		SL_SUCCESS	1

#define SlMemMov(a,b,c)		memmove(a,b,c)

struct COLOR4B { char r; char g; char b; char a; };

COLOR4B PackColor3f( float r, float g, float b );
COLOR4B PackColor4f( float a, float r, float g, float b );


#define I3D_PRIM	int

#define I3D_VC_XYZ		(1 << 0)
#define I3D_VC_XYZRHW	(1 << 1)
#define I3D_VC_DIFFUSE	(1 << 2)
#define I3D_VC_NORMAL	(1 << 3)
#define I3D_VC_UV1		(1 << 4)
#define I3D_VC_SPECULAR	(1 << 5)
#define I3D_VC_UV2		(1 << 6)

#endif
// *******************************************



/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

#ifdef _DIRECT_X
	struct IDirect3DIndexBuffer8;
#endif

class I3DINDXBUF
    {
    public:

#ifdef _DIRECT_X
    IDirect3DIndexBuffer8*  mD3dIb;    // pointer to D3D vertex buffer object
#endif

#ifdef _OPEN_GL
	UWORD*				mGLIb;
	unsigned int		mGLBufferName;
#endif
    
    int             mFirstLocked;
	int             mNum;

                    I3DINDXBUF     (bool            dynamic,            // TRUE if dynamic
                                    int             num);
                    ~I3DINDXBUF    (void);
                                   
    UWORD*          Lock           (int             num);
    UWORD*          Lock           (void);
    void            Unlock         (void);

    // index specification interface
    void            AddTri         (UWORD a, UWORD b, UWORD c);
    void            AddQuad        (UWORD a, UWORD b, UWORD c, UWORD d);

    private:

    bool            mDynamic;
    int             mLocked;
    int             mCurPos;            // current pos in the vertex array returned by Lock
    bool            mMustFlush;         // TRUE if we need to reset the vertex buffer queue
    UWORD*          mWritePtr;          // address of locked vertex buffer region
    UWORD*          mCurPtr;            // points at next vertex to be specified
    };


inline UWORD* I3DINDXBUF::Lock()
    {
    return( Lock(mNum) );
    }

inline void I3DINDXBUF::AddTri(UWORD a, UWORD b, UWORD c)
    {
    SlAssert(mCurPtr);
    *mCurPtr = a;
    mCurPtr++;
    *mCurPtr = b;
    mCurPtr++;
    *mCurPtr = c;
    mCurPtr++;
    }

inline void I3DINDXBUF::AddQuad(UWORD a, UWORD b, UWORD c, UWORD d)
    {
    SlAssert(mCurPtr);
    *mCurPtr = a;
    mCurPtr++;
    *mCurPtr = b;
    mCurPtr++;
    *mCurPtr = c;
    mCurPtr++;
    *mCurPtr = a;
    mCurPtr++;
    *mCurPtr = c;
    mCurPtr++;
    *mCurPtr = d;
    mCurPtr++;
    }

#endif  // I3DINDXBUF_H

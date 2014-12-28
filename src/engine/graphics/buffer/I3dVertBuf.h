/*----------------------------------------------------------------------------
    IDX VERT BUF

    MMH 2007/03/22: Created

    Copyright (c) 2007 Matthew Harmon
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _I3DVERTBUF_H
#define _I3DVERTBUF_H




// ************* TEST STUFF ****
// REMOVE ME
#ifndef _SIMPLE_STUFF
#define _SIMPLE_STUFF

#include <string.h>

#define UDWORD	unsigned long
#define UWORD	unsigned short

class VECTOR3 { 
public:
	VECTOR3(float vals[]) : x(vals[0]), y(vals[1]), z(vals[2])
	{}
	float x; float y; float z; 
};
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
/*                            External Variables                            */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

#ifdef _DIRECT_X
	struct IDirect3DVertexBuffer8;
#endif

class I3DINDXBUF;

class I3DVERTBUF
    {
    public:

#ifdef _DIRECT_X
    IDirect3DVertexBuffer8*  mD3dVb;    // pointer to D3D vertex buffer object
#endif

#ifdef _OPEN_GL
    char*		     mGLVb;			// Points to system memory for Vertex arrays, if using Vertex arrays
    unsigned int	 mGLBufferName;	// Name of the VBO if using VBOs	
#endif

    int             mVertSize;          // size, in bytes, of the current vertex structure
    int             mFvfFlags;          // D3D Flexible vertex format flags for this vert type
    int             mFirstLocked;       // index of first vertex locked on the previous lock
    int             mNum;

                    I3DVERTBUF     (bool            dynamic,            // TRUE if dynamic
                                    UDWORD          vertType,           // type of vertex (I3D_VT)
                                    int             num);
                    ~I3DVERTBUF    (void);
                                   
    bool            Lock           (int             numVerts);
    bool            Lock           (void);
    void            Unlock         (void);

    // vertex specification interface
    void            NextVert       (void);
    void            SetPos         (float           x, 
                                    float           y, 
                                    float           z);
    void            SetPos         (VECTOR3*        v);
    void            SetNormal      (float           x, 
                                    float           y, 
                                    float           z);
    void            SetNormal      (VECTOR3*        v);
    void            SetColor       (float           r, 
                                    float           g, 
                                    float           b);
    void            SetColor       (float           a, 
                                    float           r, 
                                    float           g, 
                                    float           b);
    void            SetColor       (COLOR4B         c);
    void            SetAlpha       (float           a);
    void            SetFog         (float           f);
    void            SetUV0         (float           u, 
                                    float           v);
    void            SetUV0         (float*          uv);
    void            SetUV1         (float           u,
                                    float           v);
    void            SetUV1         (float*          uv);

char* GetPointer()
{
	return mCurPtr;
}

void GetPos(float &x, float &y, float &z)
{
    x = *(float*)(mCurPtr+mOffsetPos+0);
    y = *(float*)(mCurPtr+mOffsetPos+4);
    z = *(float*)(mCurPtr+mOffsetPos+8);
}

VECTOR3 GetPos()
{
	return *(VECTOR3*)(mCurPtr+mOffsetPos);
}

void GetUVs(float &u, float &v)
{
    u = *(float*)(mCurPtr+mOffsetUV0+0);
    v = *(float*)(mCurPtr+mOffsetUV0+4);
}

void GetNormal(float &x, float &y, float &z)
{
    x = *(float*)(mCurPtr+mOffsetNormal+0);
    y = *(float*)(mCurPtr+mOffsetNormal+4);
    z = *(float*)(mCurPtr+mOffsetNormal+8);
}

void GetColor(COLOR4B &c)
{
    c = *(COLOR4B*)(mCurPtr+mOffsetDiffuse);
}



#ifdef _DIRECT_X
    private:	// Render Function needs offsets for OpenGL version
#endif    

    UDWORD          mVertType;          // bitflags that describe vertex type
    int             mOffsetPos;         // offset of position from start of vertex
    int             mOffsetNormal;      // offset of normal from start of vertex
    int             mOffsetDiffuse;     // offset of diffuse color from start of vertex
    int             mOffsetSpecular;    // offset of specular color from start of vertex
    int             mOffsetUV0;         // offset of first UV coordinate from start of vertex
    int             mOffsetUV1;         // offset of second UV coordinate start of vertex
    
#ifndef _DIRECT_X
    private:
#endif    

    bool            mDynamic;
    int             mLocked;
    int             mCurPos;            // current pos in the vertex array returned by Lock
    bool            mMustFlush;         // TRUE if we need to reset the vertex buffer queue
    char*           mWritePtr;          // address of locked vertex buffer region
    char*           mCurPtr;            // points at next vertex to be specified

    void            ConstructVert  (void);
    };


void I3dSelectBuffers  (I3DVERTBUF* vb,
                        I3DINDXBUF* ib);
int I3dRenderPrimIdx2  (I3D_PRIM    type,
                        int         count,                 // number of primitives to render
                        int         numVerts);

int I3dRenderPrim		(I3D_PRIM	type,
						 int		count,
						 int		numVerts);


/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/

inline bool I3DVERTBUF::Lock()
    {
    return( Lock(mNum) );
    }

inline void I3DVERTBUF::NextVert()
    {
    mCurPtr += mVertSize;
    }

inline void I3DVERTBUF::SetPos(float x, float y, float z)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_XYZ);
    *(float*)(mCurPtr+mOffsetPos+0) = x;
    *(float*)(mCurPtr+mOffsetPos+4) = y;
    *(float*)(mCurPtr+mOffsetPos+8) = z;
    }

inline void I3DVERTBUF::SetPos(VECTOR3* v)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_XYZ);
    V3Copy((VECTOR3*)(mCurPtr+mOffsetPos), v);
    }

inline void I3DVERTBUF::SetNormal(float x, float y, float z)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_NORMAL);
    *(float*)(mCurPtr+mOffsetNormal+0) = x;
    *(float*)(mCurPtr+mOffsetNormal+4) = y;
    *(float*)(mCurPtr+mOffsetNormal+8) = z;
    }

inline void I3DVERTBUF::SetNormal(VECTOR3* v)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_NORMAL);
    V3Copy((VECTOR3*)(mCurPtr+mOffsetNormal), v);
    }

inline void I3DVERTBUF::SetColor(float r, float g, float b)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_DIFFUSE);
    *(COLOR4B*)(mCurPtr+mOffsetDiffuse) = PackColor3f(r, g, b);
    }

inline void I3DVERTBUF::SetColor(float a, float r, float g, float b)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_DIFFUSE);
    *(COLOR4B*)(mCurPtr+mOffsetDiffuse) = PackColor4f(a, r, g, b);
    }

inline void I3DVERTBUF::SetColor(COLOR4B c)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_DIFFUSE);
    *(COLOR4B*)(mCurPtr+mOffsetDiffuse) = c;
    }

inline void I3DVERTBUF::SetAlpha(float a)
    {
    SlAssert(mCurPtr);
    }

inline void I3DVERTBUF::SetFog(float f)
    {
    SlAssert(mCurPtr);
    }

inline void I3DVERTBUF::SetUV0(float u, float v)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_UV1);
    *(float*)(mCurPtr+mOffsetUV0+0) = u;
    *(float*)(mCurPtr+mOffsetUV0+4) = v;
    }

inline void I3DVERTBUF::SetUV0(float* uv)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_UV1);
    SlMemMov(mCurPtr+mOffsetUV0, uv, 2 * sizeof(float));
    }

inline void I3DVERTBUF::SetUV1(float u, float v)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_UV2);
    *(float*)(mCurPtr+mOffsetUV1+0) = u;
    *(float*)(mCurPtr+mOffsetUV1+4) = v;
    }

inline void I3DVERTBUF::SetUV1(float* uv)
    {
    SlAssert(mCurPtr);
    SlAssert(mVertType & I3D_VC_UV2);
    SlMemMov(mCurPtr+mOffsetUV1, uv, 2 * sizeof(float));
    }


#endif  // I3DVERTBUF_H

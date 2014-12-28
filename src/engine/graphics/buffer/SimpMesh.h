/*----------------------------------------------------------------------------
    SIMP MESH
    Simple mesh interface.

    MMH 01/??/04: Created.

    Copyright (c) 2004 Matthew Harmon
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include "I3dVertBuf.h"
#include "I3dIndxBuf.h"

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _SIMPMESH_H
#define _SIMPMESH_H

/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

#define SIMPMESHNEW


/*--------------------------------------------------------------------------*/
/*                       Class Structure Defined                            */
/*--------------------------------------------------------------------------*/

class I3DVERTBUF;
class I3DINDXBUF;
class SIMPMESH
    {
    private:
    #if _DEBUG
    char        mName[10];
    #endif _DEBUG

#ifdef SIMPMESHNEW
    I3DVERTBUF* mVertexBuf;
    I3DINDXBUF* mIndexBuf;
#else
    I3D_VB*     mVertexBuf;
    I3D_IB*     mIndexBuf;
#endif
#ifndef SIMPMESHNEW
    UWORD*      mIdxPtr;
#endif

    public:
    int         mNumTriangles;
    int         mNumVertices;
    BOUNDBOX3   bb;

                SIMPMESH       (void);
                ~SIMPMESH      (void);
                               
    void        Create         (int             vertType,
                                int             numVertices,   
                                int             numTriangles);
    void        Destroy        (void);
    // vertex buffer management
    void        VertBufLock    (int             numVerts);
    void        VertBufLock    (void);
    void        VertBufUnlock  (void);
    // vertex specification interface
    void        NextVert       (void);
    void        SetPos         (float           x, 
                                float           y, 
                                float           z);
    void        SetPos         (VECTOR3*        v);
    void        SetNormal      (float           x, 
                                float           y, 
                                float           z);
    void        SetNormal      (VECTOR3*        v);
    void        SetColor       (float           r, 
                                float           g, 
                                float           b);
    void        SetColor       (float           a, 
                                float           r, 
                                float           g, 
                                float           b);
    void        SetColor       (COLOR4B         c);
    void        SetAlpha       (float           a);
    void        SetFog         (float           f);
    void        SetUV0         (float           u, 
                                float           v);
    void        SetUV0         (float*          uv);
    void        SetUV1         (float           u,
                                float           v);
    void        SetUV1         (float*          uv);

    // index buffer management
    UWORD*      IndexBufLock   (int             numIndices);
    UWORD*      IndexBufLock   (void);
    void        IndexBufUnlock (void);
    // index specification interface
    void        AddTri         (UWORD           a,
                                UWORD           b,
                                UWORD           c);
    void        AddQuad        (UWORD           a,
                                UWORD           b,
                                UWORD           c,
                                UWORD           d);
#ifdef SIMPMESHNEW
    I3DINDXBUF* GetIndexBuffer (void);
    I3DVERTBUF* GetVertexBuffer(void);
#else
    I3D_IB*     GetIndexBuffer (void);
    I3D_VB*     GetVertexBuffer(void);
#endif
    void        Render         (void);
    void        RenderFan      (void);
    };


/*--------------------------------------------------------------------------*/
/*                     Initialize Class Variables                           */
/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/
/*                       Inline Instance Methods                            */
/*--------------------------------------------------------------------------*/


#ifdef SIMPMESHNEW

inline I3DINDXBUF* SIMPMESH::GetIndexBuffer()
    {
    return(mIndexBuf);
    }

inline I3DVERTBUF* SIMPMESH::GetVertexBuffer()
    {
    return(mVertexBuf);
    }

inline void SIMPMESH::VertBufLock(int numVerts)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->Lock(numVerts);
    }

inline void SIMPMESH::VertBufLock()
    {
    SlAssert(mVertexBuf);
    mVertexBuf->Lock();
    }

inline void SIMPMESH::VertBufUnlock()
    {
    SlAssert(mVertexBuf);
    mVertexBuf->Unlock();
    }

inline void SIMPMESH::NextVert()
    {
    SlAssert(mVertexBuf);
    mVertexBuf->NextVert();
    }

inline void SIMPMESH::SetPos(float x, float y, float z)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetPos(x, y, z);
    }

inline void SIMPMESH::SetPos(VECTOR3* v)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetPos(v);
    }

inline void SIMPMESH::SetNormal(float x, float y, float z)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetNormal(x, y, z);
    }

inline void SIMPMESH::SetNormal(VECTOR3* v)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetNormal(v);
    }

inline void SIMPMESH::SetColor(float r, float g, float b)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetColor(r, g, b);
    }

inline void SIMPMESH::SetColor(float a, float r, float g, float b)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetColor(a, r, g, b);
    }

inline void SIMPMESH::SetColor(COLOR4B c)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetColor(c);
    }

inline void SIMPMESH::SetAlpha(float a)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetAlpha(a);
    }

inline void SIMPMESH::SetFog(float f)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetFog(f);
    }

inline void SIMPMESH::SetUV0(float u, float v)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetUV0(u, v);
    }

inline void SIMPMESH::SetUV0(float* uv)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetUV0(uv);
    }

inline void SIMPMESH::SetUV1(float u, float v)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetUV1(u, v);
    }

inline void SIMPMESH::SetUV1(float* uv)
    {
    SlAssert(mVertexBuf);
    mVertexBuf->SetUV1(uv);
    }

inline UWORD* SIMPMESH::IndexBufLock(int numIndices)
    {
    SlAssert(mIndexBuf);
    return(mIndexBuf->Lock(numIndices));
    }

inline UWORD* SIMPMESH::IndexBufLock()
    {
    SlAssert(mIndexBuf);
    return(mIndexBuf->Lock());
    }

inline void SIMPMESH::IndexBufUnlock()
    {
    SlAssert(mIndexBuf);
    mIndexBuf->Unlock();
    }

inline void SIMPMESH::AddTri(UWORD a, UWORD b, UWORD c)
    {
    SlAssert(mIndexBuf);
    mIndexBuf->AddTri(a, b, c);
    }

inline void SIMPMESH::AddQuad(UWORD a, UWORD b, UWORD c, UWORD d)
    {
    SlAssert(mIndexBuf);
    mIndexBuf->AddQuad(a, b, c, d);
    }

#else


inline I3D_IB* SIMPMESH::GetIndexBuffer()
    {
    return(mIndexBuf);
    }

inline I3D_VB* SIMPMESH::GetVertexBuffer()
    {
    return(mVertexBuf);
    }

inline void SIMPMESH::AddTri(UWORD a, UWORD b, UWORD c)
    {
    SlAssertEx(mIdxPtr, "Mesh index buffer not locked!");
    *mIdxPtr = a;
    mIdxPtr++;
    *mIdxPtr = b;
    mIdxPtr++;
    *mIdxPtr = c;
    mIdxPtr++;
    }

inline void SIMPMESH::AddQuad(UWORD a, UWORD b, UWORD c, UWORD d)
    {
    AddTri(a, b, c);
    AddTri(a, c, d);
    }

#endif


#endif  // SIMPMESH_H

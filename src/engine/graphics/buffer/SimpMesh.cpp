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

#include    <stdio.h>
#include    <math.h>
#include    <string.h>
#if 0	// REMOVE THIS LINE!
#include    "SysLib.h"              // Matt's operating system library
#include    "G2dMin.h"              // graphics library    
#include    "I2d.h"                 // graphics library    
#include    "I3d2.h"                // 3D rasterizer library
#endif // REMOVE THIS LINE!
#include    "SimpMesh.h"

/*--------------------------------------------------------------------------*/
/*                           Instance Methods                               */
/*--------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
    SIMPMESH CONSTRUCTOR
*/

SIMPMESH::SIMPMESH()
    {
    #if _DEBUG
    strcpy(mName, "SIMPMESH");
    #endif
    mVertexBuf    = NULL;
    mIndexBuf     = NULL;
    mNumTriangles = 0;
    mNumVertices  = 0;
#ifndef SIMPMESHNEW
    mIdxPtr       = NULL;
#endif
    }


/*----------------------------------------------------------------------------
    SIMPMESH CONSTRUCTOR
*/

SIMPMESH::~SIMPMESH()
    {
    Destroy();
    }


/*----------------------------------------------------------------------------
    SIMPMESH CREATE
*/

void SIMPMESH::Create(
    int             vertType,
    int             numVertices,   
    int             numTriangles)
    {
    SlAssert(numVertices);
    SlAssert(numTriangles);
    SlAssert(mVertexBuf == NULL);
    SlAssert(mIndexBuf == NULL);

    mNumTriangles = numTriangles;
    mNumVertices  = numVertices;
   
    // create buffers that will hold all clumps
#ifdef SIMPMESHNEW
    mVertexBuf = new I3DVERTBUF(SL_FALSE, vertType, mNumVertices);
    mIndexBuf  = new I3DINDXBUF(SL_FALSE, mNumTriangles * 3);
#else
    mVertexBuf = I3dVertBufCreate (I3D_VBSTATIC, vertType, mNumVertices);
    mIndexBuf  = I3dIndexBufCreate(I3D_VBSTATIC, mNumTriangles * 3);
#endif
    }


/*----------------------------------------------------------------------------
    SIMPMESH DESTROY
*/

void SIMPMESH::Destroy()
    {
#ifdef SIMPMESHNEW
    SlSafeDelete(mVertexBuf);
    SlSafeDelete(mIndexBuf);
#else
    // trap meshes that never had Create() called on them
    // SlAssertEx(mVertexBuf, "SimpMesh never created.");
    // SlAssertEx(mIndexBuf,  "SimpMesh never created.");

    if (mVertexBuf)
        {
        I3dVertBufDestroy(mVertexBuf);
        mVertexBuf = NULL;
        }

    if (mIndexBuf)
        {
        I3dIndexBufDestroy(mIndexBuf);
        mIndexBuf = NULL;
        }
#endif
    }


#ifndef SIMPMESHNEW
/*----------------------------------------------------------------------------
    VERT BUF LOCK
*/

void SIMPMESH::VertBufLock(
    int     numVerts)
    {
    I3dVertBufLock(mVertexBuf, numVerts);
    }

void SIMPMESH::VertBufLock()
    {
    I3dVertBufLock(mVertexBuf, mNumVertices);
    }


/*----------------------------------------------------------------------------
    VERT BUF UNLOCK
*/

void SIMPMESH::VertBufUnlock(void)
    {
    I3dVertBufUnlock(mVertexBuf);
    }


/*----------------------------------------------------------------------------
    INDEX BUF LOCK
*/

UWORD* SIMPMESH::IndexBufLock(
    int     numIndices)
    {
    SlAssertEx(mIdxPtr == NULL, "Mesh index buffer already locked!");
    mIdxPtr = I3dIndexBufLock(mIndexBuf, numIndices);
    return(mIdxPtr);
    }

UWORD* SIMPMESH::IndexBufLock()
    {
    SlAssertEx(mIdxPtr == NULL, "Mesh index buffer already locked!");
    mIdxPtr =I3dIndexBufLock(mIndexBuf, mNumTriangles*3);
    return(mIdxPtr);
    }


/*----------------------------------------------------------------------------
    SIMPMESH
*/

void SIMPMESH::IndexBufUnlock(void)
    {
    SlAssert(mIndexBuf);
    I3dIndexBufUnlock(mIndexBuf);
    mIdxPtr = NULL;
    }
#endif


/*----------------------------------------------------------------------------
    SIMPMESH RENDER
*/

void SIMPMESH::Render()
    {
    SlAssert(mVertexBuf);
    SlAssert(mIndexBuf);
#ifdef SIMPMESHNEW
    I3dSelectBuffers(mVertexBuf, mIndexBuf);
    I3dRenderPrimIdx2(I3D_TRILIST, mNumTriangles, mNumVertices);
#else
    I3dVertBufSelect(mVertexBuf);
    I3dIndexBufSelect(mIndexBuf);
    I3dRenderPrimIdxEx(I3D_TRILIST, mNumTriangles, mNumVertices, 0);
#endif
    }


/*----------------------------------------------------------------------------
    SIMPMESH RENDER FAN
*/

void SIMPMESH::RenderFan()
    {
    SlAssert(mVertexBuf);
#ifdef SIMPMESHNEW
#else
    I3dVertBufSelect(mVertexBuf);
    I3dRenderPrim(I3D_TRIFAN, mNumTriangles);
#endif
    }
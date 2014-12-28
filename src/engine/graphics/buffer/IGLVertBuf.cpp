/*----------------------------------------------------------------------------
    GL I3D VERT BUF

    MMH 2007/03/22: Created

    Copyright (c) 2007 Matthew Harmon
    contact: Matthew Harmon Matt@MatthewHarmon.com
*/

/*
	TODO:
		Keep System Copy
		Write to System Copy
		Unlock by glBufferSubData-ing the locked section to the card		
*/


/* -------------------------------------------------------------------------------
 *	OpenGL Implementation of Vertex Buffers
**/




/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

// **** TEMPORARY ****
#include "SDL.h" 
#include "SDL_opengl.h"
// **** /TEMPORARY ****

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include    "I3dVertBuf.h"

//#if 0	// REMOVE THIS LINE
#include    "I3dIndxBuf.h"
//#endif	// REMOVE THIS LINE


typedef unsigned long DWORD;


// **** TEMPORARY *****
COLOR4B PackColor3f( float r, float g, float b )
{
	COLOR4B ret;
	ret.r = (char)(255 * r);
	ret.g = (char)(255 * g);
	ret.b = (char)(255 * b);
	ret.a = 0;
	return ret;
}

COLOR4B PackColor4f( float a, float r, float g, float b )
{
	COLOR4B ret;
	ret.r = (char)(255 * r);
	ret.g = (char)(255 * g);
	ret.b = (char)(255 * b);
	ret.a = (char)(255 * a);
	return ret;
}

void V3Copy( VECTOR3* dest, VECTOR3* src )
{
	*dest = *src;
}
// **** /TEMPORARY *****


#if 0	// REMOVE THIS LINE
#include    "SysLib.h"              // Matt's operating system library
#include    "G2dMin.h"              // Matt's graphics library    
#include   "I3d2.h"                // 3D rasterizer library
#endif	// REMOVE THIS LINE


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

enum LOCK_FLAGS
    {
    VB_FLUSH,
    VB_APPEND
    };

/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                              Function Macros                             */    
/*--------------------------------------------------------------------------*/

 
/*--------------------------------------------------------------------------*/
/*                            External Variables                            */
/*--------------------------------------------------------------------------*/

// Whether Vertex arrays or Vertex Buffer Objects should be used
extern bool		gGLSupportsVBO;	


// The Function Pointers if VBOs are Used
extern PFNGLGENBUFFERSARBPROC		glGenBuffers;					// VBO Name Generation Procedure
extern PFNGLBINDBUFFERARBPROC		glBindBuffer;					// VBO Bind Procedure
extern PFNGLBUFFERDATAARBPROC		glBufferData;					// VBO Data Loading Procedure
extern PFNGLDELETEBUFFERSARBPROC	glDeleteBuffers;				// VBO Deletion Procedure
extern PFNGLMAPBUFFERARBPROC		glMapBuffer;
extern PFNGLUNMAPBUFFERARBPROC		glUnmapBuffer;
extern PFNGLBUFFERSUBDATAARBPROC	glBufferSubData;
#ifndef GL_VERSION_1_2
extern PFNGLDRAWRANGEELEMENTSPROC	glDrawRangeElements;
#endif

/*--------------------------------------------------------------------------*/
/*                             Local Prototypes                             */    
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                           External Prototypes                            */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                             Global Variables                             */
/*--------------------------------------------------------------------------*/


I3DVERTBUF* gVbSelected = NULL;
I3DINDXBUF* gIbSelected = NULL;


/*----------------------------------------------------------------------------
    I3DVERTBUF CONSTRUCTOR
*/

I3DVERTBUF::I3DVERTBUF(
    bool            dynamic,            // TRUE if dynamic
    UDWORD          vertType,           // type of vertex (I3D_VT)
    int             num)                // num verts to allocate
    {

    int             usage;

    // initialize the object
    mFirstLocked = -1;
    mLocked      = SL_FALSE;
    mNum         = num;
    mDynamic     = dynamic;
    mVertType    = vertType;
    mCurPos      = 0;
    mGLVb		 = NULL;
    
    mMustFlush   = SL_TRUE;
    ConstructVert();

    if (mDynamic)
        {
		usage = GL_STREAM_DRAW;
        }
    else
        {
		usage = GL_STATIC_DRAW;
        }

	if ( gGLSupportsVBO )
		{
		// Get a new VBO name
		glGenBuffers( 1, &mGLBufferName );
		// Create interleaved VBO
		glBindBuffer( GL_ARRAY_BUFFER, mGLBufferName );
		glBufferData( GL_ARRAY_BUFFER, mNum * mVertSize, NULL, usage );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		}
	
	// allocate system memory for the vertex array
	mGLVb = new char[mNum * mVertSize];
    
    }

/*----------------------------------------------------------------------------
    I3DVERTBUF DESTRUCTOR
*/

I3DVERTBUF::~I3DVERTBUF()
    {
    // unlock, just in case
    if (mLocked)
        Unlock();

	if ( gGLSupportsVBO )
		glDeleteBuffers( 1, &mGLBufferName );
	
	delete[] mGLVb;

    // if we were selected, null out the current selection
    if (gVbSelected == this)
        gVbSelected = NULL;
    }


/*----------------------------------------------------------------------------
    GL VERT BUF CONSTRUCT VERT
    Constructs the vertex offsets that will allow vertex specification routines
    to work properly.
*/

void I3DVERTBUF::ConstructVert()
    {
    mVertSize = 0;

    if (mVertType & I3D_VC_XYZ)
        {
        mOffsetPos = 0;
        mVertSize += sizeof(float) * 3;
        }

    SlAssert( !(mVertType & I3D_VC_XYZRHW) );

    if (mVertType & I3D_VC_NORMAL)
        {     
        mOffsetNormal = mVertSize;
        mVertSize += sizeof(float) * 3;        
        }

    if (mVertType & I3D_VC_DIFFUSE)
        {
        mOffsetDiffuse = mVertSize;
        mVertSize += sizeof(DWORD);
        }

    SlAssert( !(mVertType & I3D_VC_SPECULAR) );
/*      {
        mOffsetSpecular = mVertSize;
        mVertSize += sizeof(DWORD);
        }	*/

    if (mVertType & I3D_VC_UV1)
        {
        mOffsetUV0 = mVertSize;
        mVertSize += sizeof(float) * 2;
        }
    else if (mVertType & I3D_VC_UV2)
        {
        mOffsetUV0 = mVertSize;
        mVertSize += sizeof(float) * 2;
        mOffsetUV1 = mVertSize;
        mVertSize += sizeof(float) * 2;
        }
    }


/*----------------------------------------------------------------------------
    GL VERT BUF LOCK
	Maps a vertex buffer for writing.
*/

bool I3DVERTBUF::Lock(
    int         count)                  // number of verts to lock
    {

    SlAssert(mLocked == 0);

    // abort if already locked
    if (mLocked)
        return(SL_FALSE);

    // abort if lock request is too big
    if (count > mNum)
        return(SL_FALSE);

    // force a flush if there are not enough verts left and we need to reset to the beginning of the buffer
    if (count + mCurPos >= mNum)
        mMustFlush = SL_TRUE;

    // flush the contents - reset the vertex queue to 0 if necessary
    if (!mDynamic)
        {
        mCurPos = 0;
        }
    else
        {
        if (mMustFlush)
            {
            mMustFlush = SL_FALSE;
            mCurPos    = 0;
            }
        }


	mWritePtr = mGLVb + mCurPos * mVertSize;

	mLocked++;
	mFirstLocked = mCurPos;
	mCurPos += count;
	mCurPtr = mWritePtr;
	
	return(SL_TRUE);
    }

/*----------------------------------------------------------------------------
    GL VERT BUF UNLOCK
    Unmaps a previously locked vertex buffer.
*/

void I3DVERTBUF::Unlock()
    {
    
    SlAssert(mLocked == 1);

    if (!mLocked)
        return;

	if ( gGLSupportsVBO )
		{	
		ptrdiff_t offset;
		int size;
		void* dataPtr;

		// Update data stored in VBO
		SlAssert( glIsBuffer( mGLBufferName ) );
		glBindBuffer( GL_ARRAY_BUFFER, mGLBufferName );

		offset	= (ptrdiff_t)(mFirstLocked * mVertSize);
		size	= (mCurPos - mFirstLocked) * mVertSize;
		dataPtr = (void*)(mGLVb + offset);

		if ( size < 0 )
			size = mNum - mCurPos;

		glBufferSubData( GL_ARRAY_BUFFER, offset, size, dataPtr );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		}

    // leave firstLocked alone as that is where rendering will start.
    mLocked--;
    mWritePtr = NULL;
    mCurPtr   = NULL;

    }



/*----------------------------------------------------------------------------
    I3D SELECT BUFFERS
    Choose which vertex buffer is currently used for rendering.
*/

void I3dSelectBuffers(
    I3DVERTBUF* vb,                     // vertex buffer to select
    I3DINDXBUF* ib)                     // vertex buffer to select
    {
    SlAssert(vb);
    SlAssert(ib);

    // abort if we're already using this vertex buffer
    if (vb != gVbSelected)
        {
		void* pointer;
		char* position;

        gVbSelected = vb;


		// Bind GL Pointers
		if ( gGLSupportsVBO )
			{
			SlAssert( glIsBuffer( gVbSelected->mGLBufferName ) );
			glBindBuffer( GL_ARRAY_BUFFER, gVbSelected->mGLBufferName );
			position = 0;
			}
		else
			{
			position = gVbSelected->mGLVb;
			}

		// Position in the Vertex Buffer (or in memory)
		// Should this be set to FirstLocked?
		position += gVbSelected->mFirstLocked;	


		// ************ Setup Interleaved GL Pointers	***********

		// Clear Any Previously Used GL Pointers
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );

		// Set the current GL Pointers
		if (gVbSelected->mVertType & I3D_VC_XYZ)
			{
			glEnableClientState( GL_VERTEX_ARRAY );
			pointer = (void*)(position + gVbSelected->mOffsetPos);
			glVertexPointer( 3, GL_FLOAT, gVbSelected->mVertSize, pointer );
			}
		
		if (gVbSelected->mVertType & I3D_VC_NORMAL)
			{
			glEnableClientState( GL_NORMAL_ARRAY );
			pointer = (void*)(position + gVbSelected->mOffsetNormal);
			glNormalPointer( GL_FLOAT, gVbSelected->mVertSize, pointer );
			}

		if (gVbSelected->mVertType & I3D_VC_DIFFUSE)	// 3 or 4 components?, unsigned short?
			{
			glEnableClientState( GL_COLOR_ARRAY );
			pointer = (void*)(position + gVbSelected->mOffsetDiffuse);
			glColorPointer( 4, GL_UNSIGNED_BYTE, gVbSelected->mVertSize, pointer );
			}

		if (gVbSelected->mVertType & I3D_VC_UV1)
			{
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			pointer = (void*)(position + gVbSelected->mOffsetUV0);
			glTexCoordPointer( 2, GL_FLOAT, gVbSelected->mVertSize, pointer );
			}
		else if (gVbSelected->mVertType & I3D_VC_UV2)
			{
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			pointer = (void*)(position + gVbSelected->mOffsetUV0);
			glTexCoordPointer( 4, GL_FLOAT, gVbSelected->mVertSize, pointer );
			}

        }

    if (ib != gIbSelected && ib)
        {
        gIbSelected = ib;

		if ( gGLSupportsVBO )
			{
			SlAssert( glIsBuffer( gIbSelected->mGLBufferName ) );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIbSelected->mGLBufferName );
			}
		else
			{
			// Indices are directly passed to GL when using Vertex Buffers
			}
        }

    }


/*----------------------------------------------------------------------------
    GL RENDER PRIM
*/

int I3dRenderPrim(
    I3D_PRIM    type,
    int         count,                 // number of primitives to render
    int         numVerts)
    {

    SlAssert(gVbSelected != NULL);
    SlAssert(count);
    SlAssert(numVerts);

	// ************************************************************************
	// TODO:
	// Should convert type -> GLenum type
	//	and should multiply count * the number of vertices for that type
	// ************************************************************************
	// Draws elements using current vertex array & range of the index array
	glDrawArrays( GL_TRIANGLES, 0, count*3 );

	if ( gGLSupportsVBO )
		{
		// Clears out the buffer to prevent any strange effects on
		//	gl functions that accept pointer arguments
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		gVbSelected = NULL;
		}

    return(SL_SUCCESS);
    }


//#if 0
/*----------------------------------------------------------------------------
    GL RENDER PRIM IDX
*/

int I3dRenderPrimIdx2(
    I3D_PRIM    type,
    int         count,                 // number of primitives to render
    int         numVerts)
    {
	void* indexOffset;

    SlAssert(gVbSelected != NULL);
    SlAssert(gIbSelected != NULL);
    SlAssert(count);
    SlAssert(numVerts);


	//	********	Draw	***************

	// Should convert type -> GLenum type
	// Draws elements using current vertex array & range of the index array

    if ( gGLSupportsVBO )
		{
		indexOffset = (void*)(gIbSelected->mFirstLocked * sizeof(UWORD));
		}
    else
		{
		indexOffset = (void*)(gIbSelected->mGLIb + gIbSelected->mFirstLocked * sizeof(UWORD));
		}    

	glDrawElements( GL_TRIANGLES, count*3, GL_UNSIGNED_SHORT, indexOffset );

	if ( gGLSupportsVBO )
		{
		// Clears out the buffers to prevent any strange effects on
		//	gl functions that accept pointer arguments
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		gIbSelected = NULL;
		gVbSelected = NULL;
		}

    return(SL_SUCCESS);
    }


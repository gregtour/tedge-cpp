/*----------------------------------------------------------------------------
    GL I3D INDX BUF

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
 *	OpenGL Implementation of Index Buffers
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


#if 0	// REMOVE THIS LINE
#include    "SysLib.h"              // Matt's operating system library
#include    "G2dMin.h"              // Matt's graphics library    
#include   "I3d2.h"                // 3D rasterizer library
#endif	// REMOVE THIS LINE
#include    "I3dIndxBuf.h"

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
extern bool			gGLSupportsVBO;	
extern I3DINDXBUF*	gIbSelected;


// The Function Pointers if VBOs are Used
extern PFNGLGENBUFFERSARBPROC		glGenBuffers;					// VBO Name Generation Procedure
extern PFNGLBINDBUFFERARBPROC		glBindBuffer;					// VBO Bind Procedure
extern PFNGLBUFFERDATAARBPROC		glBufferData;					// VBO Data Loading Procedure
extern PFNGLDELETEBUFFERSARBPROC	glDeleteBuffers;				// VBO Deletion Procedure
extern PFNGLMAPBUFFERARBPROC		glMapBuffer;
extern PFNGLUNMAPBUFFERARBPROC		glUnmapBuffer;
extern PFNGLBUFFERSUBDATAARBPROC	glBufferSubData;


/*--------------------------------------------------------------------------*/
/*                             Local Prototypes                             */    
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                           External Prototypes                            */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                             Global Variables                             */
/*--------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
    I3DINDXBUF CONSTRUCTOR
*/

I3DINDXBUF::I3DINDXBUF(
    bool            dynamic,            // TRUE if dynamic
    int             num)                // num verts to allocate
    {
    int             usage;

    // initialize the object
    mFirstLocked = -1;
    mLocked      = SL_FALSE;
    mNum         = num;
    mDynamic     = dynamic;
    mCurPos      = 0;
    mMustFlush   = SL_TRUE;
	mGLIb		 = NULL;

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
		// Create VBO
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mGLBufferName );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, mNum * sizeof(UWORD), NULL, usage );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		}

	// allocate system memory for the vertex array and the local copy of the VBO
	mGLIb = new UWORD[mNum];

    }


/*----------------------------------------------------------------------------
    I3DINDXBUF DESTRUCTOR
*/

I3DINDXBUF::~I3DINDXBUF()
    {
    // unlock, just in case
    if (mLocked)
        Unlock();

	if ( gGLSupportsVBO )
		glDeleteBuffers( 1, &mGLBufferName );
	
	delete[] mGLIb;

    // if we were selected, null out the current selection
    if (gIbSelected == this)
        gIbSelected = NULL;
    }


/*----------------------------------------------------------------------------
    GL VERT BUF LOCK
    Locks a vertex buffer for writing.
*/

UWORD* I3DINDXBUF::Lock(
    int         count)                  // number of verts to lock
    {

    SlAssert(mLocked == 0);

    // abort if already locked
    if (mLocked)
        return(NULL);

    // abort if lock request is too big
    if (count > mNum)
        return(NULL);

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


	mWritePtr = mGLIb + mCurPos * sizeof(UWORD);

	mLocked++;
	mFirstLocked = mCurPos;
	mCurPos += count;
	mCurPtr = mWritePtr;
	
	return(mWritePtr);
	}


/*----------------------------------------------------------------------------
    GL VERT BUF UNLOCK
    Unlocks a previously locked vertex buffer.
*/

void I3DINDXBUF::Unlock()
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

		offset	= (ptrdiff_t)(mFirstLocked * sizeof(UWORD));
		size	= (mCurPos - mFirstLocked) * sizeof(UWORD);
		dataPtr = (void*)(mGLIb + offset);

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

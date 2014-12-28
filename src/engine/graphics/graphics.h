#ifndef _GRAPHICS_H
#define _GRAPHICS_H

// #define _QUADRIC

#include "SDL.h" 
#include "SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "../../engine/entity/camera.h"
#include "../engine.h"
#include "../entity/graphicalObj.h"
#include "../physics/common.h"

#define FLAT_SHADED

#ifndef PI
#	define	PI		(3.141592f)
#endif
#define HALF_PI	(1.570796f)
#ifndef RADS
#	define RADS	(PI/180.0f)
#endif
#ifndef DEGREES
#define DEGREES		(180.0f/PI)
#endif
#ifndef ROOT_TWO
#	define ROOT_TWO	(1.414214f)
#endif

#define VIEW_DISTANCE			300.0f
#define VIEW_DISTANCE_SQUARED	(VIEW_DISTANCE*VIEW_DISTANCE)
//#define VIEW_DISTANCE			100.0f
//#define VIEW_DISTANCE_SQUARED	(100.0f*100.0f)

#if 1

#include <GL/glext.h>


extern PFNGLGENBUFFERSARBPROC glGenBuffers;					// VBO Name Generation Procedure
extern PFNGLBINDBUFFERARBPROC glBindBuffer;					// VBO Bind Procedure
extern PFNGLBUFFERDATAARBPROC glBufferData;					// VBO Data Loading Procedure
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;				// VBO Deletion Procedure
extern PFNGLMAPBUFFERARBPROC glMapBuffer;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBuffer;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubData;
#ifndef GL_VERSION_1_2
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
#endif

extern bool gGLSupportsVBO;

#endif

//#define _IMMEDIATE_MODE
#define VBO
//#define INDICES



// Graphics Options:
//#define NO_FOG
//#define SHOW_FPS
//#define MAX_LIGHTS	2
//#define WIREFRAME_RENDERING
//#define LOW_QUALITY_GEOMETRY
//#define LOW_RESOLUTION_TEXTURES
//#define SCR_WIDTH 	1024
//#define SCR_HEIGHT 	768
//#define SCR_DEPTH		16
//#define FULLSCREEN

/*
#ifndef _DEBUG
#	define FULLSCREEN
#else
#	define SHOW_FPS
#endif
*/
//#define FULLSCREEN
#define SHOW_FPS

#define PSP_QUALITY

#ifdef PSP_QUALITY
#	define MAX_LIGHTS	4
#	define LOW_QUALITY_GEOMETRY
//#	define LOW_RESOLUTION_TEXTURES
#ifdef _PSP
#	define SCR_WIDTH	480
#	define SCR_HEIGHT	272
#else
//#	define SCR_WIDTH	1280
//#	define SCR_HEIGHT	725
//#define SCR_WIDTH	1024
//#define SCR_HEIGHT	580
//#define SCR_WIDTH 1920
//#define SCR_HEIGHT 1080 

//#define SCR_WIDTH 800
//#define SCR_HEIGHT 600

//#define SCR_WIDTH 1024
//#define SCR_HEIGHT 768

#define SCR_WIDTH 1280
#define SCR_HEIGHT 1024

//#define SCR_WIDTH	480
//#define SCR_HEIGHT	272
#endif
#	define SCR_DEPTH	32
#endif


//#define HIGH_QUALITY

//#ifdef HIGH_QUALITY
//#	define MAX_LIGHTS	8
//#	define SCR_DEPTH	32
//#	define SCR_WIDTH	480
//#	define SCR_HEIGHT	272
//#	define SCR_WIDTH	800
//#	define SCR_HEIGHT	600
//#endif



#ifndef FULLSCREEN
	#define SCR_FLAGS 	(SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_HWSURFACE | SDL_HWACCEL ) // SDL_HWPALETTE SDL_HWSURFACE SDL_HWACCEL SDL_FULLSCREEN
#else
	#define SCR_FLAGS 	(SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_HWSURFACE | SDL_HWACCEL | SDL_FULLSCREEN)
#endif


int 	GraphicsStartup();
int 	GraphicsShutdown();
int 	GameRender();
void	BindCamera( CCamera* c );

SVector3 HSV(float h, float s, float v);

class CLight;

extern CLinkedList<IGraphicalObj>	gGraphicalObjList;
extern CLinkedList<CLight>			gLights;

#endif

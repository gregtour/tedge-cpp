// Graphics.cpp

#include "../engine.h"
#include "graphics.h"
#include "../common/list.h"
#include "../entity/camera.h"
#include "../entity/graphicalObj.h"
#include "particle.h"
#include <cmath>
#include <cstring>
#include "texture.h"
#include "buffer/I3dVertBuf.h"
//#include "../common/profile.h"
#include <iostream>

#ifdef _PSP
#include "clipping/clipping.h"
#endif


SDL_Surface* gScreen;
CCamera* gCamera = NULL;
CTexture*		gSky;

CLinkedList<IGraphicalObj>	gGraphicalObjList;
CLinkedList<CLight>			gLights;


#ifdef USE_GL_EXT
#include <GL/glext.h>


PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;					// VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBuffer = NULL;					// VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferData = NULL;					// VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = NULL;				// VBO Deletion Procedure
PFNGLMAPBUFFERARBPROC glMapBuffer = NULL;
PFNGLUNMAPBUFFERARBPROC glUnmapBuffer = NULL;
PFNGLBUFFERSUBDATAARBPROC glBufferSubData = NULL;
#ifndef GL_VERSION_1_2
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = NULL;
#endif



// Get a pointer to an OpenGL extension
void *gltGetExtensionPointer(const char* extensionName)
{
	return (void *)SDL_GL_GetProcAddress(extensionName);
//	return (void *)wglGetProcAddress(szExtensionName);
}


///////////////////////////////////////////////////////////////////////////////
// This function determines if the named OpenGL Extension is supported
// Returns 1 or 0
int gltIsExtSupported(const char *extension)
	{
	GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;

	extensions = (GLubyte *)glGetString(GL_EXTENSIONS);

	const char* extensions_string = (const char *)extensions;

	start = extensions;
	for (;;) 
		{
		where = (GLubyte *) strstr((const char *) start, extension);

		if (!where)
			break;
    
		terminator = where + strlen(extension);
    
		if (where == start || *(where - 1) == ' ') 
			{
			if (*terminator == ' ' || *terminator == '\0') 
				return 1;
			}
		start = terminator;
		}
	return 0;
	}


#ifdef _WIN32

///////////////////////////////////////////////////////////////////////////////

// Win32 Only, check for WGL extension

#include <gl/wglext.h>

int gltIsWGLExtSupported(HDC hDC, const char *szExtension)
	{
	GLubyte *extensions = NULL;

	const GLubyte *start;
	GLubyte *where, *terminator;
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

    // Just look or the entry point
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

    if(wglGetExtensionsStringARB == NULL)
        return 0;

	where = (GLubyte *) strchr(szExtension, ' ');

	if (where || *szExtension == '\0')
		return 0;

	extensions = (GLubyte *)wglGetExtensionsStringARB(hDC);

	start = extensions;
	for (;;) 
		{
		where = (GLubyte *) strstr((const char *) start, szExtension);
		if (!where)
			break;
		terminator = where + strlen(szExtension);
		if (where == start || *(where - 1) == ' ') 
			{
			if (*terminator == ' ' || *terminator == '\0') 
				return 1;
			}
		start = terminator;
		}
	return 0;
	}

  #endif


#endif

bool gGLSupportsVBO = false;

//#define _IMMEDIATE_MODE
//#define VBO
//#define INDICES


void BindCamera( CCamera* c )
{
	gCamera = c;
}


#ifdef USE_GL_EXT
void LoadExtensions()
{
	// extensions
	const GLubyte *version;
	GLboolean glVersion15 = GL_FALSE;	// 1.5	- VBOs
	GLboolean glVersion12 = GL_FALSE;	// 1.2	- DrawRangeElements

	// Make sure required functionality is available!
	version = glGetString(GL_VERSION);
	if (version) {
		if ((version[0] == '1') && (version[1] == '.') &&
			(version[2] >= '5') && (version[2] <= '9'))
		{
			glVersion15 = GL_TRUE;
		}

		if ((version[0] == '1') && (version[1] == '.') &&
			(version[2] >= '2') && (version[2] <= '9'))
		{
			glVersion12 = GL_TRUE;
		}
	}

#ifndef GL_VERSION_1_2
	if (glVersion12)
		glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)gltGetExtensionPointer("glDrawRangeElements");
	else
		glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)gltGetExtensionPointer("glDrawRangeElements");
#endif

#ifdef	VBO
	if (!glVersion15 && !gltIsExtSupported("GL_ARB_vertex_buffer_object"))
	{
	}

	// Load the function pointers
	if (glVersion15)
	{
		glBindBuffer = (PFNGLBINDBUFFERARBPROC)gltGetExtensionPointer("glBindBuffer");
		glBufferData = (PFNGLBUFFERDATAARBPROC)gltGetExtensionPointer("glBufferData");
		glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)gltGetExtensionPointer("glBufferSubData");
		glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)gltGetExtensionPointer("glDeleteBuffers");
		glGenBuffers = (PFNGLGENBUFFERSARBPROC)gltGetExtensionPointer("glGenBuffers");
		glMapBuffer = (PFNGLMAPBUFFERARBPROC)gltGetExtensionPointer("glMapBuffer");
		glUnmapBuffer = (PFNGLUNMAPBUFFERARBPROC)gltGetExtensionPointer("glUnmapBuffer");
	}
	else
	{
		glBindBuffer = (PFNGLBINDBUFFERARBPROC)gltGetExtensionPointer("glBindBufferARB");
		glBufferData = (PFNGLBUFFERDATAARBPROC)gltGetExtensionPointer("glBufferDataARB");
		glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)gltGetExtensionPointer("glBufferSubDataARB");
		glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)gltGetExtensionPointer("glDeleteBuffersARB");
		glGenBuffers = (PFNGLGENBUFFERSARBPROC)gltGetExtensionPointer("glGenBuffersARB");
		glMapBuffer = (PFNGLMAPBUFFERARBPROC)gltGetExtensionPointer("glMapBufferARB");
		glUnmapBuffer = (PFNGLUNMAPBUFFERARBPROC)gltGetExtensionPointer("glUnmapBufferARB");
	}

	if (!glBindBuffer || !glBufferData || !glBufferSubData || 
		!glDeleteBuffers || !glGenBuffers || !glMapBuffer || !glUnmapBuffer)
	{
	}
	else
	{
		gGLSupportsVBO = true;
	}
#endif
}
#endif

int shadowSize = 1024;
GLuint shadowTextureID;

int GraphicsStartup()
{		
	SDL_ShowCursor( 0 );

	gScreen = SDL_SetVideoMode( SCR_WIDTH, SCR_HEIGHT, SCR_DEPTH, SCR_FLAGS );
	if (!gScreen) {
		printf("Error setting video mode.");
		return 0;
	}
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_WM_SetCaption( gameTitle, gameTitle );
	
	glShadeModel( GL_SMOOTH );

	glFrontFace( GL_CCW );

	glEnable( GL_CULL_FACE );
//	glDisable( GL_CULL_FACE );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth( 1.0f );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

#ifdef WIREFRAME_RENDERING
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

//	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glViewport( 0, 0, SCR_WIDTH, SCR_HEIGHT );

	glMatrixMode( GL_PROJECTION );
	gluPerspective( 55.0f , SCR_WIDTH/(GLfloat)SCR_HEIGHT , 1.0f, VIEW_DISTANCE );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

//	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);
#ifdef USE_GL_EXT
	LoadExtensions();
#endif
#ifdef USING_SKYBOX
	gSky = CManagedTexture::Load( &gResourceManager, "sky.jpg"  );
#endif

#ifdef WITH_SHADOWS
    glGenTextures(1, &shadowTextureID);
    glBindTexture(GL_TEXTURE_2D, shadowTextureID);
#endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	std::cout << "Vertex Buffer Objects " << (gGLSupportsVBO ? "Supported" : "Not Supported") << std::endl;
	return 1;
}

int GraphicsShutdown()
{
	if (CParticleEmitter::gParticleVerts)
	{
		delete CParticleEmitter::gParticleVerts;
		CParticleEmitter::gParticleVerts = NULL;
	}
	return 1;
}

void Sky()
{
	gSky->Bind();
    glDepthMask(GL_FALSE);
	float r = 1.0f;
	float cz = -0.0f,cx = 1.0f;

	glBegin(GL_QUADS);	
		glTexCoord2f(cx, cz); glVertex3f(-r ,1.0f,-r);
		glTexCoord2f(cx,  cx); glVertex3f(-r,1.0f,r);
		glTexCoord2f(cz,  cx); glVertex3f( r,1.0f,r); 
		glTexCoord2f(cz, cz); glVertex3f( r ,1.0f,-r);
	glEnd();
 
	// Common Axis Z - BACK side
	glBegin(GL_QUADS);		
		glTexCoord2f(cx,cz);  glVertex3f(-r,-1.0f,-r);
		glTexCoord2f(cx,cx);  glVertex3f(-r,-1.0f, r);
		glTexCoord2f(cz,cx);  glVertex3f( r,-1.0f, r); 
		glTexCoord2f(cz,cz);  glVertex3f( r,-1.0f,-r);
	glEnd();
 
	// Common Axis X - Left side
	glBegin(GL_QUADS);		
		glTexCoord2f(cx,cx); glVertex3f(-1.0f, -r, r);	
		glTexCoord2f(cz,cx); glVertex3f(-1.0f,  r, r); 
		glTexCoord2f(cz,cz); glVertex3f(-1.0f,  r,-r);
		glTexCoord2f(cx,cz); glVertex3f(-1.0f, -r,-r);		
	glEnd();
 
	// Common Axis X - Right side
	glBegin(GL_QUADS);		
		glTexCoord2f( cx,cx); glVertex3f(1.0f, -r, r);	
		glTexCoord2f(cz, cx); glVertex3f(1.0f,  r, r); 
		glTexCoord2f(cz, cz); glVertex3f(1.0f,  r,-r);
		glTexCoord2f(cx, cz); glVertex3f(1.0f, -r,-r);
	glEnd();
 
	// Common Axis Y - Draw Up side
	glBegin(GL_QUADS);		
		glTexCoord2f(cz, cz); glVertex3f( r, -r,1.0f);
		glTexCoord2f(cx, cz); glVertex3f( r,  r,1.0f); 
		glTexCoord2f(cx, cx); glVertex3f(-r,  r,1.0f);
		glTexCoord2f(cz, cx); glVertex3f(-r, -r,1.0f);
	glEnd();
 
	// Common Axis Y - Down side
	glBegin(GL_QUADS);		
		glTexCoord2f(cz,cz);  glVertex3f( r, -r,-1.0f);
		glTexCoord2f( cx,cz); glVertex3f( r,  r,-1.0f); 
		glTexCoord2f( cx,cx); glVertex3f(-r,  r,-1.0f);
		glTexCoord2f(cz, cx); glVertex3f(-r, -r,-1.0f);
	glEnd();


	  gSky->Unbind();
    glDepthMask(GL_TRUE);
}

void ShadowMap()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // orthogonal projection
	glOrtho( 0.0, 100.0, (1000.0/SCR_WIDTH*SCR_HEIGHT), 0.0, -1.0, 1000.0 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // sun's position
    glTranslatef(0.0f, -100.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
   // glTranslatef(500.0f, 0.0f, 500.0f);


	CListEntry<IGraphicalObj>* entity = gGraphicalObjList.GetFirst();

	/*		Render Normal		*/
//	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		// Render it if its an IGraphicalObj* w/ Render() function
		IGraphicalObj* o = entity->data; //dynamic_cast<IGraphicalObj*>(entity->data);
//		if ( o != NULL )
		{
			glPushMatrix();	// Save Modelview Matrix @ Camera Position
			o->Render();
			glPopMatrix();	// Restore it
		}
		
		entity = entity->GetNext();
	}


}

int GameRender()
{
    CListEntry<IGraphicalObj>* entity;

    GLfloat lightPos[4] = {0.0f, 1.0f, 0.0f, 0.0f};
    GLfloat ambientLight_dim[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuseLight_dim[4] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat ambientLight_bright[4] = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat diffuseLight_bright[4] = {0.8f, 0.8f, 0.8f, 1.0f};

//    glEnable(GL_CULL_FACE);
//	glFrontFace( GL_CW );

	/*		Reset for new Rendering Pass	*/
/*	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 55.0f , SCR_WIDTH/(GLfloat)SCR_HEIGHT , 1.0f , VIEW_DISTANCE );
    //glOrtho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0f, 400.0f);
*/
	glMatrixMode( GL_MODELVIEW );
	//glClear( GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/*	Set to Camera View	*/
	if ( gCamera )
	{
		gCamera->Render();
	}
    
#if USING_SKYBOX
    // dry skybox
	glPushMatrix();
	SVector3 pos;
	gCamera->GetPosition(&pos.x, &pos.y, &pos.z);
	glTranslatef(pos.x,pos.y,pos.z);
	glScalef(2.0f,2.0f,2.0f);
    glDisable(GL_CULL_FACE);
	Sky();
    glEnable(GL_CULL_FACE);
	glPopMatrix();

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
#endif
#ifdef WITH_SHADOWS
	glClear( GL_DEPTH_BUFFER_BIT );


   // generate shadow map
    float lightToSceneDistance;
    float nearPlane;
    float fieldOfView;

    float lightModelview[16];
    float lightProjection[16];

    lightToSceneDistance = 200.0f;
    nearPlane = lightToSceneDistance - 150.0f;
    fieldOfView = 16000.0f / lightToSceneDistance;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

   glOrtho(-50.0f, 50.0f, -50.0f, 50.0f, 0.0f, 200.0f);
//    gluPerspective( fieldOfView , 1.0f , nearPlane , nearPlane + 300.0f );
    glGetFloatv(GL_PROJECTION_MATRIX, lightProjection);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    SVector3 light_pos(pos.x + 1, pos.y + 7, pos.z + 2);
    SVector3 up_v = CrossProduct(light_pos, pos);
    up_v.Normalize();
    gluLookAt(light_pos.x, light_pos.y, light_pos.z, pos.x, pos.y, pos.z, up_v.x, up_v.y, up_v.z); 


//    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
//    glTranslatef(0.0f, 100.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightModelview);
    glViewport(0, 0, shadowSize, shadowSize);

    glShadeModel(GL_FLAT);
    
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_NORMALIZE);
    glColorMask(0, 0, 0, 0);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.62f, 0.0f);

	// render from light's perspective	
	entity = gGraphicalObjList.GetFirst();
	while ( entity )
	{
		IGraphicalObj* o = entity->data;
		glPushMatrix();	
		o->Render();
		glPopMatrix();
		entity = entity->GetNext();
	}

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowTextureID);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, shadowSize, shadowSize, 0);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glColorMask(1, 1, 1, 1);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glViewport( 0, 0, SCR_WIDTH, SCR_HEIGHT );

    // set up texture matrix for shadow map projection
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(0.5f, 0.5f, 0.5f);
    glScalef(0.5f, 0.5f, 0.5f);
    glMultMatrixf(lightProjection);
    glMultMatrixf(lightModelview);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable( GL_DEPTH_TEST );

    // draw unlit scene
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective( 55.0f , SCR_WIDTH/(GLfloat)SCR_HEIGHT , 1.0f , VIEW_DISTANCE );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gCamera) gCamera->Render();

    glLightfv(GL_LIGHT3, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight_dim);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight_dim);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glEnable(GL_LIGHT3);

	//		Render Normal		
	entity = gGraphicalObjList.GetFirst();
	while ( entity )
	{
		IGraphicalObj* o = entity->data;
		glPushMatrix();	
		o->Render();
		glPopMatrix();
		entity = entity->GetNext();
	}

    glAlphaFunc(GL_GREATER, 0.9f);
    glEnable(GL_ALPHA_TEST);

//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    GLfloat sPlane[4] = {1.0f, 0.0f, 0.0f, 0.0f};
    GLfloat tPlane[4] = {0.0f, 1.0f, 0.0f, 0.0f};
    GLfloat rPlane[4] = {0.0f, 0.0f, 1.0f, 0.0f};
    GLfloat qPlane[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowTextureID);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
            GL_COMPARE_R_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // set up the eye plane for projecting the shadow map on the scene
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);
    glTexGenfv(GL_S, GL_EYE_PLANE, sPlane);
    glTexGenfv(GL_T, GL_EYE_PLANE, tPlane);
    glTexGenfv(GL_R, GL_EYE_PLANE, rPlane);
    glTexGenfv(GL_Q, GL_EYE_PLANE, qPlane);
#endif

    // draw lit scene
/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective( 55.0f , SCR_WIDTH/(GLfloat)SCR_HEIGHT , 1.0f , VIEW_DISTANCE );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gCamera) gCamera->Render();
*/
/*  glLightfv(GL_LIGHT3, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight_bright);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight_bright);
    glEnable(GL_LIGHT3);*/
#ifdef WITH_SHADOWS
    glActiveTexture(GL_TEXTURE0);
#endif

	//		Render Normal		
	entity = gGraphicalObjList.GetFirst();
	while ( entity )
	{
		IGraphicalObj* o = entity->data;
		glPushMatrix();	
		o->Render();
		glPopMatrix();
		entity = entity->GetNext();
	}

	/*		Render Alpha		*/
	entity = gGraphicalObjList.GetLast(); 
	while ( entity )
	{
		IGraphicalObj* o = entity->data;
		glPushMatrix();	
		o->RenderAlpha();
		glPopMatrix();	
		entity = entity->GetPrev();
	}

/*	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0.0, 1.0, (1.0/SCR_WIDTH*SCR_HEIGHT), 0.0, -1.0, 1.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	entity = gGraphicalObjList.GetFirst(); //gEntities.GetFirst();
	while ( entity )
	{
		// Render it if its an IGraphicalObj* w/ RenderOrtho() function
		IGraphicalObj* o = entity->data; //dynamic_cast<IOrthoGraphicalObj*>(entity->data);
			o->RenderOrtho();

		entity = entity->GetNext();
	}

	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
*/

	glFinish();
	SDL_GL_SwapBuffers( );	

	return 1;
}


#if 0
int GameRender()
{

/* Render Order:
Render()		-Solid Graphics		(Level, Characters, Etc)
RenderAlpha()	-Blended Graphics	(Shadows, Particles, Effects)
RenderOrtho()	-Overlays			(HUD, Menu)
*/
    ProfileStartClock(P_RENDER);

	/*		Reset for new Rendering Pass	*/
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//gluPerspective( 55.0f , SCR_WIDTH/(GLfloat)SCR_HEIGHT , 1.0f , VIEW_DISTANCE );
    glOrtho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0f, 400.0f);

	glMatrixMode( GL_MODELVIEW );
	//glClear( GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//glLoadIdentity();


	//glPushMatrix();
	
	//glDisable(GL_DEPTH_BUFFER);

	/*	Set to Camera View	*/
	if ( gCamera )
	{
		gCamera->Render();
	}

	glPushMatrix();
	SVector3 pos;
	gCamera->GetPosition(&pos.x, &pos.y, &pos.z);
	glTranslatef(pos.x,pos.y,pos.z);
	glScalef(2.0f,2.0f,2.0f);
	Sky();
	glPopMatrix();

#ifdef _PSP		// Clipping
	begin_frame();
#endif

	CListEntry<IGraphicalObj>* entity = gGraphicalObjList.GetFirst();

	/*		Render Normal		*/
//	CListEntry<CEntity>* entity = gEntities.GetFirst();
	while ( entity )
	{
		// Render it if its an IGraphicalObj* w/ Render() function
		IGraphicalObj* o = entity->data; //dynamic_cast<IGraphicalObj*>(entity->data);
//		if ( o != NULL )
		{
			glPushMatrix();	// Save Modelview Matrix @ Camera Position
			o->Render();
			glPopMatrix();	// Restore it
		}
		
		entity = entity->GetNext();
	}

	/*		Render Alpha		*/
	entity = gGraphicalObjList.GetLast(); //gEntities.GetLast();
	while ( entity )
	{
		// Render it if its an IGraphicalObj* w/ RenderAlpha() function
		IGraphicalObj* o = entity->data; //dynamic_cast<IAlphaGraphicalObj*>(entity->data);
//		if ( o != NULL )
		{
			glPushMatrix();	// Save Modelview Matrix @ Camera Position
			o->RenderAlpha();
			glPopMatrix();	// Restore it
		}
		
		entity = entity->GetPrev();
	}

	/*	Setup for Orthographic Rendering	*/
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0.0, 1.0, (1.0/SCR_WIDTH*SCR_HEIGHT), 0.0, -1.0, 1.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	/*		Render Ortho		*/
	entity = gGraphicalObjList.GetFirst(); //gEntities.GetFirst();
	while ( entity )
	{
		// Render it if its an IGraphicalObj* w/ RenderOrtho() function
		IGraphicalObj* o = entity->data; //dynamic_cast<IOrthoGraphicalObj*>(entity->data);
//		if ( o != NULL )
		{
			// glLoadIdentity();
			// glTranslatef( 0.0f, 0.0f, -1.0f );
			o->RenderOrtho();
		}
		entity = entity->GetNext();
	}

	/*	Reset Projection Matrix		*/
	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);

    ProfileStopClock(P_RENDER);
    ProfileStartClock(P_VSYNC);

	glFinish();
	SDL_GL_SwapBuffers( );	

    ProfileStopClock(P_VSYNC);
	return 1;
}
#endif

SVector3 HSV(float h, float s, float v)
{
	int hi = (int)(h/60)%6;
	float f = h/60.0f-(int)(h/60.0f);
	float p = v * (1.0f - s);
	float q = v * (1.0f - f * s);
	float t = v * (1.0f - (1.0f - f) * s);
	switch (hi)
	{
	case 1:
		return SVector3(q,v,p);
	case 2:
		return SVector3(p,v,t);
	case 3:
		return SVector3(p,q,v);
	case 4:
		return SVector3(t,p,v);
	case 5:
		return SVector3(v,p,q);
	default:
	case 0:
		return SVector3(v,t,p);
	}
}


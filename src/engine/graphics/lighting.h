#ifndef _LIGHTING_H
#define _LIGHTING_H

#include "graphics.h"

class CLight
{
public:
	CLight();
	CLight( GLfloat ambientLight[], GLfloat diffusiveLight[], GLfloat position[] );
	CLight( GLfloat ambientLight[], GLfloat diffusiveLight[], GLfloat position[], GLfloat attenuation[] );

	void Spotlight( GLfloat spotDirection[], GLfloat spotExponent, GLfloat spotCutoff );
	void SetSpotlight( bool spotlight );
	void SetSpotDirection( GLfloat spotDirection[] );
	void SetSpotExponent( GLfloat spotExponent );
	void SetSpotCutoff( GLfloat spotCutoff );

	void SetAmbient( GLfloat ambientLight[] );
	void SetDiffusive( GLfloat diffusiveLight[] );
	void SetPosition( GLfloat position[] );
	void SetAttenuation( GLfloat attenuation[] );
	void Apply( int lightNumber );

	GLfloat mPosition[4];

private:
	GLfloat mAmbient[4];
	GLfloat mDiffusive[4];
	GLfloat mAttenuation[3];
	bool mSpotlight;
	GLfloat mSpotDirection[3];
	GLfloat mSpotExponent;
	GLfloat mSpotCutoff;
};

void	DisableLighting();
void	AmbientLighting( GLfloat globalAmbient[] );
void	EnableLighting( CLinkedList<CLight>* lights, float x, float y, float z );
void	EnableLighting( CLinkedList<CLight>* lights, float x, float z );

#endif

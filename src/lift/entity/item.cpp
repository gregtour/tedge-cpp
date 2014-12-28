#include "item.h"
#include "../../engine/engine.h"
#include "../main.h"
#include <string>
#include <sstream>

CItem::CItem()
{
#ifdef LOW_RESOLUTION_TEXTURES
	mTexture = CManagedTexture::Load( &gResourceManager, "data/low/crate.bmp" );
#else
	mTexture = CManagedTexture::Load( &gResourceManager, "data/crate.bmp" );
#endif
}

CItem::~CItem()
{
	CManagedTexture::Unload( &gResourceManager, mTexture );
}

void CItem::Update( float dt )
{

};

void CItem::Render()
{
/*	{
		float mX = mObject->GetPosition().x;
		float mZ = mObject->GetPosition().y;
		float x, y, z;
		gCamera->GetPosition( &x, &y, &z );
		if (  ((x-mX)*(x-mX) + (z-mZ)*(z-mZ)) > VIEW_DISTANCE_SQUARED )
			return;
	}

	gLevel->Light( mObject->GetPosition() );

	glEnable( GL_TEXTURE_2D );
	mTexture->BindTexture();


	float r = ((CPCircle*)mObject)->GetRadius() * 0.9f;
	glTranslatef( mObject->GetPosition().x, r, mObject->GetPosition().y );
*/	
	if ( mObject )
	{
		SBoundingBox b = mObject->GetBoundingBox();
		SVector3 translate = ( b.min + b.max ) / 2.0f;
		glTranslatef( translate.x, translate.y, translate.z );
		SVector3 scale = b.max - b.min;
		glScalef( scale.x, scale.y, scale.z );

		{	// Draw Box
			float r = 0.5f;
			glColor3f( 1.0f, 1.0f, 1.0f );
			glBegin(GL_QUADS);		// Draw The Cube Using quads
				glNormal3f( 0.0f, 1.0f, 0.0f );
				glVertex3f( r, r,-r);	// Top Right Of The Quad (Top)
				glVertex3f(-r, r,-r);	// Top Left Of The Quad (Top)
				glVertex3f(-r, r, r);	// Bottom Left Of The Quad (Top)
				glVertex3f( r, r, r);	// Bottom Right Of The Quad (Top)
				glNormal3f( 0.0f, -1.0f, 0.0f );
				glVertex3f( r,-r, r);	// Top Right Of The Quad (Bottom)
				glVertex3f(-r,-r, r);	// Top Left Of The Quad (Bottom)
				glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Bottom)
				glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Bottom)
				glNormal3f( 0.0f, 0.0f, 1.0f );
				glVertex3f( r, r, r);	// Top Right Of The Quad (Front)
				glVertex3f(-r, r, r);	// Top Left Of The Quad (Front)
				glVertex3f(-r,-r, r);	// Bottom Left Of The Quad (Front)
				glVertex3f( r,-r, r);	// Bottom Right Of The Quad (Front)
				glNormal3f( 0.0f, 0.0f, -1.0f );
				glVertex3f( r,-r,-r);	// Top Right Of The Quad (Back)
				glVertex3f(-r,-r,-r);	// Top Left Of The Quad (Back)
				glVertex3f(-r, r,-r);	// Bottom Left Of The Quad (Back)
				glVertex3f( r, r,-r);	// Bottom Right Of The Quad (Back)
				glNormal3f( -1.0f, 0.0f, 0.0f );
				glVertex3f(-r, r, r);	// Top Right Of The Quad (Left)
				glVertex3f(-r, r,-r);	// Top Left Of The Quad (Left)
				glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Left)
				glVertex3f(-r,-r, r);	// Bottom Right Of The Quad (Left)
				glNormal3f( 1.0f, 0.0f, 0.0f );
				glVertex3f( r, r,-r);	// Top Right Of The Quad (Right)
				glVertex3f( r, r, r);	// Top Left Of The Quad (Right)
				glVertex3f( r,-r, r);	// Bottom Left Of The Quad (Right)
				glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Right)
			  glEnd();			// End Drawing The Cube
		}
	}

  //glDisable( GL_TEXTURE_2D );
};

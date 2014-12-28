#include <math.h>
#include "monster.h"
#include "../../engine/graphics/graphics.h"
#include "../../engine/input/input.h"
#include "../../engine/engine.h"
#include "../../engine/common/log.h"

CMonster::CMonster()
{
	mTexture = NULL;
	//LoadTexture();
}

CMonster::~CMonster()
{
	if ( mTexture )
	{
		// If it isn't a managed texture, unload it the normal way
		if ( !CManagedTexture::Unload( &gResourceManager, mTexture ) )
			mTexture->Unload();
	}
}

int CMonster::AttackStrength()
{
	return 1;
}

void CMonster::Update( float dt )
{
	SVector3 force;
	force.x = cos( mRot ) * 4.0f;
	force.y = 0.0f;
	force.z = sin( mRot ) * 4.0f;
	mObject->SetVelocity(force);
	

	mRot -= 1.0f * dt;
}

void CMonster::Render()
{	
	float r = mObject->GetSphere().radius;
	//float r = ((CPCircle*)mObject)->GetRadius() * 0.9f;
	SVector3 pos = mObject->GetPosition();
	glTranslatef( pos.x, pos.y, pos.z );
	glRotatef( (GLfloat)(mRot * DEGREES), 0.0f, -1.0f, 0.0f );

	mTexture->Bind();
	
	/*	Temporary Code, Draw A Cube as a Placeholder	*/
  glBegin(GL_QUADS);		// Draw The Cube Using quads

    glColor3f(1.0f,1.0f,1.0f);

	glNormal3f( 0.0f, 1.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r,-r);	// Top Right Of The Quad (Top)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r, r,-r);	// Top Left Of The Quad (Top)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r, r, r);	// Bottom Left Of The Quad (Top)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r, r);	// Bottom Right Of The Quad (Top)

	glNormal3f( 0.0f, -1.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r,-r, r);	// Top Right Of The Quad (Bottom)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r,-r, r);	// Top Left Of The Quad (Bottom)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Bottom)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Bottom)


	glNormal3f( 0.0f, 0.0f, 1.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r, r);	// Top Right Of The Quad (Front)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r, r);	// Top Left Of The Quad (Front)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r, r);	// Bottom Left Of The Quad (Front)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r,-r, r);	// Bottom Right Of The Quad (Front)
	

	glNormal3f( 0.0f, 0.0f, -1.0f );

	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r,-r,-r);	// Top Right Of The Quad (Back)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r,-r);	// Top Left Of The Quad (Back)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r,-r);	// Bottom Left Of The Quad (Back)
	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r,-r);	// Bottom Right Of The Quad (Back)


	glNormal3f( -1.0f, 0.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(-r, r, r);	// Top Right Of The Quad (Left)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(-r, r,-r);	// Top Left Of The Quad (Left)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(-r,-r,-r);	// Bottom Left Of The Quad (Left)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f(-r,-r, r);	// Bottom Right Of The Quad (Left)

	glNormal3f( 1.0f, 0.0f, 0.0f );

	glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( r, r,-r);	// Top Right Of The Quad (Right)
	glTexCoord2f( 1.0f, 0.0f );
    glVertex3f( r, r, r);	// Top Left Of The Quad (Right)
	glTexCoord2f( 1.0f, 1.0f );
    glVertex3f( r,-r, r);	// Bottom Left Of The Quad (Right)
	glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( r,-r,-r);	// Bottom Right Of The Quad (Right)
  glEnd();			// End Drawing The Cube

  mTexture->Unbind();
}
void CMonster::LoadTexture()
{
}



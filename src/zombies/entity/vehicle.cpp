#include "vehicle.h"
#include "../game/gameInput.h"
#include "../../engine/graphics/texture.h"
#include "../../engine/graphics/model.h"
#include "jeep.h"
#include <cmath>
//#include "zombie.h"

CPObject* tires[4];

void rendercube()
{
    float vertex[] = {
        -1, -1, -1, 1, -1, -1,
        -1, -1, 1,  1, -1, 1,
        -1, 1, -1,  1, 1, -1,
        -1, 1,  1,  1, 1,  1
    };
    
    float color[] = {
        1, 0, 0,    0, 1, 0,
        0, 0, 1,    1, 0, 0,
        0, 1, 0,    0, 0, 1,
        1, 0, 0,    0, 1, 0
    };
    
    int index[] = {
        1, 2, 3, 2, 4, 3,
        7, 6, 5, 7, 8, 6,
        1, 6, 2, 1, 5, 6,
        3, 4, 8, 3, 8, 7,
        1, 3, 7, 1, 7, 5,
        2, 6, 4, 6, 8, 4
    };
    
    //glTranslatef(SCR_WIDTH/2, SCR_HEIGHT/2, -100.0f);
    //glRotatef(frame, 1, 1, 1);
    glScalef(0.2f, 0.2f, 0.2f);

    glBegin(GL_TRIANGLES);
        for (int i = 0; i < 36; i++)
        {
            int j = (index[i] - 1)*3;
            glColor4f(color[j], color[j+1], color[j+2], 1.0f);
            glVertex3f(vertex[j], vertex[j+1], vertex[j+2]);
        }
    glEnd();
}

CVehicle::CVehicle()
{
	mModel = CManagedModel::Load(&gResourceManager, "tank5.ms3d");
    mTexture = CManagedTexture::Load(&gResourceManager, "camoflauge.jpg");
	mHasJumped = false;
    SVector3 pos(500.1f, 50.0f, 499.8f);
		//player = new CVehicle();
        
		CreatePhysical(gWorld); 
		CPObject* obj = GetPhysicalObject();

		SBoundingBox bb;
		bb.min = SVector3( -4.0f, 0.0f, -4.0f );
		bb.max = SVector3( 4.0f, 3.0f, 4.0f );
		
		SSphere s;
		s.position = SVector3( 0.0f, 3.0f, 0.0f );
		s.radius = 4.0f;

		obj->SetPosition( pos );
		obj->SetSphere( s );
		obj->SetBoundingBox( bb );
		obj->SetObjectType(1 /*PLAYER_OBJ_TYPE*/);
		obj->SetCollisionMask( 2 );
		obj->SetDefaultForce( SVector3( 0.0f, 0.0f, 0.0f ) );
		obj->SetFriction( 12.0f );

	    SetRot( HALF_PI );

    for (int i = 0; i < 4; i++) 
    {
        tires[i] = 	new CPObject( true );
    	tires[i]->SetOwner(this);
        gWorld->Add(tires[i]);

        SBoundingBox b;
        b.min = SVector3(-0.25f, 0, -0.25f);
        b.max = SVector3(0.25f, 0.5f, 0.25f);
        
        SSphere s;
        s.position = SVector3(0, 0.25f, 0);
        s.radius = 0.25f;

        tires[i]->SetPosition( pos + SVector3( (i%2)*3.0f - 1.5f, 0.0f, (int)(i/2)*4.0f - 1.0f ) );
        tires[i]->SetSphere( s );
        tires[i]->SetBoundingBox( b );
        tires[i]->SetObjectType(0 /*PLAYER_OBJECT_TYPE*/);
        tires[i]->SetCollisionMask(1);
        tires[i]->SetDefaultForce( SVector3( 0.0f, -145.0f, 0.0f ) );
        tires[i]->SetFriction( 10.0f );
    }
    
    CPSpring* a = new CPSpring;
    a->a = tires[0];
    a->b = tires[1];
    a->k = 1.0f;
    a->length = 3.0f; 
    gWorld->Add(a);

    CPSpring* b = new CPSpring;
    b->a = tires[2];
    b->b = tires[3];
    b->k = 1.0f;
    b->length = 3.0f; 
    gWorld->Add(b);

    CPSpring* c = new CPSpring;
    c->a = tires[0];
    c->b = tires[2];
    c->k = 1.0f;
    c->length = 4.0f; 
    gWorld->Add(c);

    CPSpring* d = new CPSpring;
    d->a = tires[1];
    d->b = tires[3];
    d->k = 1.0f;
    d->length = 4.0f; 
    gWorld->Add(d);

    CPSpring* e = new CPSpring;
    e->a = tires[0];
    e->b = tires[3];
    e->k = 1.0f;
    e->length = sqrt(25.0f);
    gWorld->Add(e);

    CPSpring* f = new CPSpring;
    f->a = tires[1];
    f->b = tires[2];
    f->k = 1.0f;
    f->length = sqrt(25.0f);
   gWorld->Add(f);

}

CVehicle::~CVehicle()
{
}

void CVehicle::Update( float dt )
{
    t += dt;

    SVector3 avg(0.0f, 0.0f, 0.0f);
    SVector3 vel(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; i++) {
        avg = avg + tires[i]->GetPosition();
        vel = vel + tires[i]->GetVelocity();
    }
    mObject->SetPosition(avg * 0.25f + SVector3(0.0f, 1.0f, 0.0f));
    mObject->SetVelocity(vel * 0.25f);

	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		SVector3 cam = gCamera->GetPosition();

		if (pos.y < -30.0f)
			ResetPosition();

		// Analog stick
		float push = mInputState->xaxis * mInputState->xaxis + mInputState->yaxis * mInputState->yaxis;
		if (push > (0.2f) * (0.2f))
		{
			SVector3 distance = pos - cam;
			push = sqrt(push);
			float ang = mRot - atan2(distance.z, distance.x) + atan2(mInputState->yaxis, mInputState->xaxis);
			float cosAng = cos(ang);
			float sinAng = sin(ang);

			if (sinAng < -0.35f)
				MoveForward(-sinAng * push * dt);
			else if (sinAng > 0.4f)
				MoveBackward(sinAng * push * dt);

			if (fabs(cosAng) > 0.1f)
				TurnLeft(cosAng * push * dt * 1.5f);
			/*float angDif = ang + PI/2.0f;
			while (angDif > PI)
				angDif -= PI*2.0f;
			while (angDif < -PI)
				angDif += PI*2.0f;
			TurnLeft(angDif * dt);*/

		// push = push_angle + cam_angle
		// cos(push_ang + cam_ang - facing)
		// moveForward( dt * analog_push * push . facing )
		//float theta = atan2(ang.z, ang.x);
		}


		// Digital Input
		if ( mInputState->IsKeyPressed(K_UP) )
		{
			MoveForward(dt*1.0f);
		}

		if ( mInputState->IsKeyPressed(K_DOWN) )
		{
			MoveBackward(dt);
		}
		
		if ( mInputState->IsKeyPressed(K_RIGHT) )
		{
			TurnRight(dt);
		}

		if ( mInputState->IsKeyPressed(K_LEFT) )
		{
			TurnLeft(dt);
		}

		if (mInputState->IsKeyHit(K_JUMP) && !mHasJumped)
		{
			Jump();
		}
	}

	SetGrounded(false);
}

void CVehicle::Render()
{
    glDisable(GL_LIGHTING);

    SVector3 avg_pos(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; i++) {
        glPushMatrix();

        SVector3 pos = tires[i]->GetPosition();
        avg_pos = avg_pos + pos;

        glTranslatef(pos.x, pos.y, pos. z);
//        rendercube();

        glPopMatrix();
    }

    glPushMatrix();
    SVector3 pos = mObject->GetPosition();
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(4.0f, 4.0f, 4.0f);
    rendercube();
    glPopMatrix();

    SVector3 front_left = tires[0]->GetPosition();
    SVector3 front_right = tires[1]->GetPosition();
    SVector3 back_left = tires[2]->GetPosition();
    SVector3 back_right = tires[3]->GetPosition();

    SVector3 right = ((back_left - back_right) + (front_left - front_right)) * 0.5f;
    SVector3 forward = ((front_left - back_left) + (front_right - back_right)) * 0.5f;
    SVector3 up = -CrossProduct(right, forward);

    avg_pos = avg_pos * 0.25f;

    right.Normalize();
    forward.Normalize();
    up.Normalize();
/*
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(back_left.x, back_left.y, back_left.z);
        glVertex3f(front_left.x, front_left.y, front_left.z);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(back_left.x, back_left.y, back_left.z);
        glVertex3f(back_right.x, back_right.y, back_right.z);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(back_left.x, back_left.y, back_left.z);
        glVertex3f(back_left.x + up.x, back_left.y + up.y, back_left.z + up.z);
    glEnd();*/

    mRot = -atan2(forward.z, -forward.x) + 3.142f;


   glEnable(GL_LIGHTING); 
	if (mObject)
	{
		SVector3 pos = mObject->GetPosition();
		{
			float x, y, z;
			gCamera->GetPosition( &x, &y, &z );
			if ( (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y) + (z-pos.z)*(z-pos.z) > VIEW_DISTANCE_SQUARED )
				return;
		}

		glTranslatef( avg_pos.x, avg_pos.y, avg_pos.z );
        //glRotatef(270, 0.0f, 0.0f, 1.0f);
		//glRotatef(mRot * DEGREES + 90, 0.0f, -1.0f, 0.0f );

        GLfloat matrix[16] = {
            right.x, up.x, -forward.x, 0.0f,
            right.y, up.y, -forward.y, 0.0f,
            right.z, up.z, -forward.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
            //avg_pos.x, avg_pos.y, avg_pos.z, 1.0f
        };

        //glLoadMatrixf(matrix);
        glMultTransposeMatrixf(matrix);

        //glRotatef(90, 0.0f, -1.0f, 0.0f);

		glColor3f(0.4f, 0.0f, 0.0f);
        mTexture->Bind();

        glScalef(2.1f, 2.1f, 2.1f);
//		mModel->Render();
        //rendercube();

{
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    //glScalef(2.0f, 2.0f, 2.0f);
    glBegin(GL_TRIANGLES);
    glColor3f(0.5, 0.5, 0.5);
        for (int i = 0; i < sizeof(JEEP_MODEL) / sizeof(double); i += 3) {
            int j = i / 9;
            SVector3 a(JEEP_MODEL[9*j], JEEP_MODEL[9*j+1], JEEP_MODEL[9*j+2]);
            SVector3 b(JEEP_MODEL[9*j+3], JEEP_MODEL[9*j+4], JEEP_MODEL[9*j+5]);
            SVector3 c(JEEP_MODEL[9*j+6], JEEP_MODEL[9*j+7], JEEP_MODEL[9*j+8]);
            SVector3 n = CrossProduct(a - b, a - c);
            n.Normalize();

            glVertex3f(JEEP_MODEL[i], JEEP_MODEL[i+1], JEEP_MODEL[i+2]);
            glNormal3f(n.x, n.y, n.z);
            glTexCoord2f(n.x + atan2(JEEP_MODEL[i], JEEP_MODEL[i+2]), n.y*n.z + JEEP_MODEL[i+1]);
        }
    glEnd();
}

        mTexture->Unbind();
		glColor3f(1.0f, 1.0f, 1.0f);
	}
/*
    glLoadIdentity();

    glTranslatef(SCR_WIDTH/2, SCR_HEIGHT/2, 0.0);
    glRotatef(t, 0, 0, 1);
    glScalef(2.0f, 2.0f, 2.0f);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-87.0f, -50.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(87.0f, -50.0f, 0.0f);
    glEnd();
*/
/*
    mTexture->Bind();

    int frame_a, frame_b;
    float wa, wb;

    frame_a = (int)(t) % 8 + 4;
    frame_b = (frame_a - 3) % 8 + 4;
    wa = 1 - t + (int)t;
    wb = t - (int)t;

    //glLoadIdentity();

    //glTranslatef(SCR_WIDTH/2, SCR_HEIGHT/2, 0.0);
    //glRotatef(t*10, 1, 1, 1);
    glScalef(t, t, t);


    glBegin(GL_QUADS);
    glColor3f(0.5, 0.5, 0.5);
        for (int i = 0; i < 545*3*4 - 3; i+=3) 
            glVertex3f(ZOMBIE_DATA[0][i], ZOMBIE_DATA[0][i+1], ZOMBIE_DATA[0][i+2]);*/

/*      {
            glVertex3f(ZOMBIE_DATA[frame_a][i]*wa + ZOMBIE_DATA[frame_b][i]*wb, 
                ZOMBIE_DATA[frame_a][i+1]*wa + ZOMBIE_DATA[frame_b][i+1]*wb, 
                ZOMBIE_DATA[frame_a][i+2]*wa + ZOMBIE_DATA[frame_b][i+2]*wb);
        } */
/*    glEnd();

    mTexture->Unbind();*/
}

void CVehicle::TurnRight(float dt)
{
	mRot += 2.4f * dt;

	SVector3 force;
	force.x = sin(-mRot) * 200.0f * dt;
	force.z = cos(-mRot) * 200.0f * dt;
//    for (int i = 0; i < 4; i++) {
//        tires[i]->ApplyForce(force);
//    }
    tires[0]->ApplyForce(force);
    tires[1]->ApplyForce(force);
    tires[2]->ApplyForce(-force * 0.5f);
    tires[3]->ApplyForce(-force * 0.5f);
    
}

void CVehicle::TurnLeft(float dt)
{
	TurnRight(-dt);
}

void CVehicle::MoveForward(float dt)
{
	SVector3 force;
	force.x = -cos(mRot) * 280.0f * dt;
	force.z = -sin(mRot) * 280.0f * dt;
    for (int i = 0; i < 4; i++) {
        tires[i]->ApplyForce(force);
    }
	//mObject->ApplyForce(force);
}

void CVehicle::MoveBackward(float dt)
{
	MoveForward(dt * -0.7f);
}

void CVehicle::SetGrounded(bool grounded)
{
	if (grounded)
		mHasJumped = false;
	mGrounded = grounded;
}

void CVehicle::SetSafe()
{
	mSafePos = mObject->GetOldPosition();
    mGrounded = true;
    mHasJumped = false;
}

void CVehicle::Jump()
{
	if (mGrounded)
	{
		mHasJumped = true;
		SVector3 force;
		force.y = 43.0f;
		mObject->ApplyForce(force);
	}
}

void CVehicle::ResetPosition()
{
	mObject->SetPosition(mSafePos);
	mObject->SetVelocity(SVector3(0.0f, 0.0f, 0.0f));
}

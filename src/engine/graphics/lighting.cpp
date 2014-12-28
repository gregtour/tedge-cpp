#include "lighting.h"

/*	Float Wrapper Class	*/
class Float
{
public:
	Float( float f )
	{ value = f; }

	void operator=( float f )
	{ value = f; }

	float value;
};



/*		CLight Function Implementations		*/

CLight::CLight()
{
	GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	SetAmbient( ambient );

	GLfloat diffusive[] = {1.0f, 1.0f, 1.0f, 1.0f};
	SetDiffusive( diffusive );

	GLfloat position[] = {0.0f, 0.0f, 0.0f, 1.0f};
	SetPosition( position );

	GLfloat attenuation[] = {1.0f, 0.0f, 0.0f};
	SetAttenuation( attenuation );

	SetSpotlight( false );
}

CLight::CLight( GLfloat ambientLight[], GLfloat diffusiveLight[], GLfloat position[] )
{
	SetAmbient( ambientLight );
	SetDiffusive( diffusiveLight );
	SetPosition( position );

	GLfloat attenuation[] = {1.0f, 0.0f, 0.0f};
	SetAttenuation( attenuation );

	SetSpotlight( false );
}

CLight::CLight( GLfloat ambientLight[], GLfloat diffusiveLight[], GLfloat position[], GLfloat attenuation[] )
{
	SetAmbient( ambientLight );
	SetDiffusive( diffusiveLight );
	SetPosition( position );
	SetAttenuation( attenuation );
	SetSpotlight( false );
}

void CLight::Spotlight( GLfloat spotDirection[], GLfloat spotExponent, GLfloat spotCutoff )
{
	SetSpotlight( true );
	SetSpotDirection( spotDirection );
	SetSpotExponent( spotExponent );
	SetSpotCutoff( spotCutoff );
}

void CLight::SetSpotlight( bool spotlight )
{
	mSpotlight = spotlight;
}

void CLight::SetSpotDirection( GLfloat spotDirection[] )
{
	for ( int i = 0; i < 3; i++ )
		mSpotDirection[i] = spotDirection[i];
}

void CLight::SetSpotExponent( GLfloat spotExponent )
{
	mSpotExponent = spotExponent;
}

void CLight::SetSpotCutoff( GLfloat spotCutoff )
{
	mSpotCutoff = spotCutoff;
}

void CLight::SetAmbient( GLfloat ambientLight[] )
{
	for ( int i = 0; i < 4; i++ )
		mAmbient[i] = ambientLight[i];
}

void CLight::SetDiffusive( GLfloat diffusiveLight[] )
{
	for ( int i = 0; i < 4; i++ )
		mDiffusive[i] = diffusiveLight[i];
}

void CLight::SetPosition( GLfloat position[] )
{
	for ( int i = 0; i < 4; i++ )
		mPosition[i] = position[i];
}

void CLight::SetAttenuation( GLfloat attenuation[] )
{
	for ( int i = 0; i < 3; i++ )
		mAttenuation[i] = attenuation[i];
}

void CLight::Apply( int lightNumber )
{
	glEnable( GL_LIGHTING );
	
	if ( mSpotlight )
	{
		glLightf(GL_LIGHT0 + lightNumber, GL_SPOT_EXPONENT, mSpotExponent );
		glLightf(GL_LIGHT0 + lightNumber, GL_SPOT_CUTOFF, mSpotCutoff );
		glLightfv(GL_LIGHT0 + lightNumber, GL_SPOT_DIRECTION, mSpotDirection );
	} else
		glLightf(GL_LIGHT0 + lightNumber, GL_SPOT_CUTOFF, 180.0f );

	glLightf(GL_LIGHT0 + lightNumber, GL_CONSTANT_ATTENUATION, mAttenuation[0] );
	glLightf(GL_LIGHT0 + lightNumber, GL_LINEAR_ATTENUATION, mAttenuation[1] );
	glLightf(GL_LIGHT0 + lightNumber, GL_QUADRATIC_ATTENUATION, mAttenuation[2] );

	glLightfv( GL_LIGHT0 + lightNumber, GL_AMBIENT, mAmbient );
	glLightfv( GL_LIGHT0 + lightNumber, GL_DIFFUSE, mDiffusive );
	glLightfv( GL_LIGHT0 + lightNumber, GL_POSITION, mPosition );
	glEnable( GL_LIGHT0 + lightNumber );
}




/*	Global Lighting Functions	*/

/*	Disables All Lighting (Ambient and Individual)	*/
void	DisableLighting()
{
	for ( int i = 0; i < MAX_LIGHTS; i++ )
		glDisable( GL_LIGHT0 + i );

	glDisable( GL_LIGHTING );
}




/*	Enables Ambient Lighting	*/
void	AmbientLighting( GLfloat globalAmbient[] )
{
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbient );
}




/*	Enables Nearest Lights and Enables Ambient Lighting	*/
void	EnableLighting( CLinkedList<CLight>* lightsList, float x, float y, float z )
{
	int maxLights = MAX_LIGHTS;

	CLinkedList<CLight> lightsPriorityList;
	CLinkedList<Float> distancesPriorityList;

	CListEntry<CLight>* light = lightsList->GetFirst();
	while ( light )
	{
		float currentDistance = (light->data->mPosition[0] - x) * (light->data->mPosition[0] - x) + 
			(light->data->mPosition[1] - y) * (light->data->mPosition[1] - y) +
			(light->data->mPosition[2] - z) * (light->data->mPosition[2] - z);

		/*	Iterate through all the lights checked so far to find where
				to insert the current light into the list	*/
		int index = 0;
		CListEntry<Float>* distance = distancesPriorityList.GetFirst();
		while ( distance && currentDistance >= distance->data->value )
		{
			distance = distance->GetNext();
			index++;
		}

		distancesPriorityList.Add( new Float( currentDistance ), index );
		lightsPriorityList.Add( light->data, index );

		while ( lightsPriorityList.length > maxLights )
		{
			delete distancesPriorityList.GetLast()->data;
			lightsPriorityList.RemoveEntry( lightsPriorityList.GetLast() );
			distancesPriorityList.RemoveEntry( distancesPriorityList.GetLast() );
		}

		light = light->GetNext();
	}

	/*	Render the constructed PriorityList of Lights	*/
	int i = 0;
	light = lightsPriorityList.GetFirst();
	while ( light )
	{
		light->data->Apply( i );

		light = light->GetNext();
		i++;
	}

	/*	Disable All Unused Lights	*/
	for ( ; i < maxLights; i++ )
		glDisable( GL_LIGHT0 + i );

	/*	Delete the Float objects	*/
	CListEntry<Float>* floatObject = distancesPriorityList.GetFirst();
	while ( floatObject )
	{
		delete floatObject->data;
		floatObject = floatObject->GetNext();
	}
}



void	EnableLighting( CLinkedList<CLight>* lights, float x, float z )
{
	EnableLighting( lights, x, 0.0f, z );
}




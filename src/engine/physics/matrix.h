#ifndef _MATRIX_H
#define _MATRIX_H

#include "svector.h"

struct SMatrix3
{
	SMatrix3();
	SMatrix3( SVector3 x, SVector3 y, SVector3 z );
	SMatrix3( float* coordinates );

	float m[3][3];

	SVector3 operator * ( SVector3 multiple );
	SMatrix3 operator * ( SMatrix3 multiple );

	SMatrix3 FastInverse();
};


/*
#define GL_ACCELERATION
#define GUM_ACCELERATION

struct SMatrix4
{
	SMatrix();
	SMatrix( SVector3 x, SVector3 y, SVector3 z );
	SMatrix( SVector3 x, SVector3 y, SVector3 z, SVector3 translate );

	float m[4][4];

	SMatrix operator * ( SMatrix multiple );
};
*/
/*
void multMat(float *res,float *a, float *b) {


	for (int i=0;i<4;i++) {
		for (int j = 0;j < 4;j++) {
			res[i*4+j] = 0.0;
			for (int k = 0; k < 4; k++) {
				res[i*4+j] += a[i*4+k] * b[k*4+j];
			}
		}
	}
}

void multMat2(float *res, float *a, float *b) {

	glPushMatrix();

	glLoadMatrixf(b);
	glMultMatrixf(a);
	glGetFloatv(GL_MODELVIEW_MATRIX, res);

	glPopMatrix();
}

*/
#endif

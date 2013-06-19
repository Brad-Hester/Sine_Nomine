#include "Utility.h"

float Randf( float rangeMin, float rangeMax)
{
	float result = (float)rand() / (float)(RAND_MAX + 1);
	result *= (float) (rangeMax - rangeMin);
	result += rangeMin;

	return result;
}

////////////////////////////////////////////////////////////////////////////////
// DegToRad
////////////////////////////////////////////////////////////////////////////////
// Description:	Converts an angle from degrees to radian
// Parameters:	degrees	- number to be converted
// Returns:		new angle, in radian
////////////////////////////////////////////////////////////////////////////////
float DegToRad(float degrees)
{
	return degrees * PI / 180.0f;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//					WARNING: HERE BE SLEEPLESS NIGHTS						  //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool PixelFromWorldPoint(float worldPoint[3], ICamera* pCam, I3DEngine* pEngine, int* pX, int* pY)
{
	const float fieldOfViewMin = PI / 3.4f;

	float fovX;
	float fovY;

	float width = static_cast<float>(pEngine->GetWidth());
	float height = static_cast<float>(pEngine->GetHeight());

	pEngine->GetWindow();

	if(width > height)
	{
		fovY = fieldOfViewMin;
		fovX = atanf( width * tanf(fovY * 0.5f) / height ) * 2.0f;
	}
	else
	{
		fovX = fieldOfViewMin;
		fovY = atanf( height * tanf(fovX * 0.5f) / width ) * 2.0f;
	}

	float cameraMatrix[4][4];
	pCam->GetMatrix(&cameraMatrix[0][0]);
	
	float viewMatrix[4][4];
	InverseAffine(cameraMatrix, viewMatrix);
	
	float cameraPoint[3];
	TransformPoint(viewMatrix, worldPoint, cameraPoint);

	cameraPoint[X] /= tanf( fovX * 0.5f );
	cameraPoint[Y] /= tanf( fovY * 0.5f );

	if(cameraPoint[Z] <= 0)
	{
		return false;
	}

	cameraPoint[X] /= cameraPoint[Z];
	cameraPoint[Y] /= cameraPoint[Z];

	*pX = static_cast<int>((cameraPoint[X] + 1.0f) * width * 0.5f);
	*pY = static_cast<int>((1.0f - cameraPoint[Y]) * height * 0.5f);

	return true;
}

void TransformPoint(const float matrix[4][4], const float inPoint[3], float outPoint[3])
{
	outPoint[X] = inPoint[X]*matrix[0][0] + inPoint[Y]*matrix[1][0] + inPoint[Z]*matrix[2][0] + matrix[3][0];
	outPoint[Y] = inPoint[X]*matrix[0][1] + inPoint[Y]*matrix[1][1] + inPoint[Z]*matrix[2][1] + matrix[3][1];
	outPoint[Z] = inPoint[X]*matrix[0][2] + inPoint[Y]*matrix[1][2] + inPoint[Z]*matrix[2][2] + matrix[3][2];
}

// Return the inverse of given matrix assuming only that it is an affine matrix
void InverseAffine(const float in[4][4], float out[4][4])
{
	// Calculate determinant of upper left 3x3
	float det0 = in[1][1] * in[2][2] - in[1][2] * in[2][1];
	float det1 = in[1][2] * in[2][0] - in[1][0] * in[2][2];
	float det2 = in[1][0] * in[2][1] - in[1][1] * in[2][0];
	float det = in[0][0] * det0 + in[0][1] * det1 + in[0][2] - det2;

	// Calculate inverse of upper left 3x3
	float invDet = 1.0f / det;
	out[0][0] = invDet * det0;
	out[1][0] = invDet * det1;
	out[2][0] = invDet * det2;

	out[0][1] = invDet * (in[2][1]*in[0][2] - in[2][2]*in[0][1]);
	out[1][1] = invDet * (in[2][2]*in[0][0] - in[2][0]*in[0][2]);
	out[2][1] = invDet * (in[2][0]*in[0][1] - in[2][1]*in[0][0]);
		  
	out[0][2] = invDet * (in[0][1]*in[1][2] - in[0][2]*in[1][1]);
	out[1][2] = invDet * (in[0][2]*in[1][0] - in[0][0]*in[1][2]);
	out[2][2] = invDet * (in[0][0]*in[1][1] - in[0][1]*in[1][0]);

	// Transform negative translation by inverted [3]x[3] to get inverse
	out[3][0] = -in[3][0]*out[0][0] - in[3][1]*out[1][0] - in[3][2]*out[2][0];
	out[3][1] = -in[3][0]*out[0][1] - in[3][1]*out[1][1] - in[3][2]*out[2][1];
	out[3][2] = -in[3][0]*out[0][2] - in[3][1]*out[1][2] - in[3][2]*out[2][2];

	// Fill in right column for affine matrix
	out[0][3] = 0.0f;
	out[1][3] = 0.0f;
	out[2][3] = 0.0f;
	out[3][3] = 0.0f;
}
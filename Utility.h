#ifndef __Utility_H
#define __Utility_H

#include <iostream>
#include <sstream>
#include "TL-Engine.h"

using namespace std;
using namespace tle;

enum eDimensions {X, Y, Z};

const float PI = 3.14159265;

float Randf( float rangeMin, float rangeMax);
float DegToRad(float degrees);
bool PixelFromWorldPoint(float worldPoint[3], ICamera* pCam, I3DEngine* pEngine, int* pX, int* pY);
void InverseAffine(const float in[4][4], float out[4][4]);
void TransformPoint(const float matrix[4][4], const float inPoint[3], float outPoint[3]);

#endif
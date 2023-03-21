#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Math.h"

class Transform
{
public:
	vec3 translation;
	quat rotation;
	vec3 scale;

	Transform() : scale(1.f) {};
	Transform(const mat4 &modelMatrix)
	{
		mat4 localMatrix(modelMatrix);
		if (localMatrix[3][3] == 0)
		{
			LOG("failed to decompose matrix !");
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			for (int i = 0; i < 4; i++)
			{
				localMatrix[i][j] /= localMatrix[3][3];
			}
		}
		
	}
};

#endif
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
	Transform(const mat4 &ModelMatrix)
	{
		mat4 LocalMatrix(ModelMatrix);
		if (epsilonEqual(LocalMatrix[3][3], 0, numeric_limits<float>::min()))
		{
			LOG("failed to decompose matrix !");
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				LocalMatrix[i][j] /= LocalMatrix[3][3];
			}
		}
		mat4 PerspectiveMatrix(LocalMatrix);
		for (int i = 0; i < 3; i++)
		{
			PerspectiveMatrix[i][3] = 0;
			PerspectiveMatrix[3][3] = 1;
		}
		if (epsilonEqual(determinant(PerspectiveMatrix), 0, numeric_limits<float>::epsilon()))
		{
			LOG("failed to decompose matrix !");
			return;
		}
		if(
			epsilonNotEqual(LocalMatrix[0][3], 0, numeric_limits<float>::epsilon()) ||
			epsilonNotEqual(LocalMatrix[1][3], 0, numeric_limits<float>::epsilon()) ||
			epsilonNotEqual(LocalMatrix[2][3], 0, numeric_limits<float>::epsilon()))
		{
			// rightHandSide is the right hand side of the equation.
			vec4 RightHandSide;
			RightHandSide[0] = LocalMatrix[0][3];
			RightHandSide[1] = LocalMatrix[1][3];
			RightHandSide[2] = LocalMatrix[2][3];
			RightHandSide[3] = LocalMatrix[3][3];
			mat4 InversePerspectiveMatrix = inverse(PerspectiveMatrix);//   inverse(PerspectiveMatrix, inversePerspectiveMatrix);
			mat4 TransposedInversePerspectiveMatrix = transpose(InversePerspectiveMatrix);//   transposeMatrix4(inversePerspectiveMatrix, transposedInversePerspectiveMatrix);
		}
	}
};

#endif
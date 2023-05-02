#ifndef MAT4_H
#define MAT4_H

#include "math_fwd.h"
#include "vec3.h"
#include "vec4.h"
#include "quat.h"

#define MAT4_LOOP 4

struct mat4
{
	vec4 row[4];
	mat4()
	{
		for (int i = 0; i < MAT4_LOOP; i++)
		{
			for (int j = 0; j < MAT4_LOOP; j++)
			{
				if (i == j)
				{
					row[i][j]=1.f;
				}
				else
				{
					row[i][j]=0.f;
				}
			}
		}
	}
	mat4(const initializer_list<vec4> &l)
	{
		for (int i = 0; i < MAT4_LOOP; i++)
		{
			row[i] = *(l.begin()+i);
		}
	}
	mat4(const quat &q);
	quat ToQuat() const;
	mat4 operator+(const mat4& m) const;
	mat4 operator-(const mat4& m) const;
	mat4 operator*(const mat4& m) const;
	vec4 operator*(const vec4& v) const;
	mat4 operator*(float f) const;
	mat4 operator/(const mat4& m) const;
	bool operator==(const mat4& v) const;
	bool operator!=(const mat4& v) const;
	vec4& operator[](int i);
	vec4 const& operator[](int i) const;
	vec4 column(int i) const;
	mat4 transpose() const;
	mat4 inverse() const;
	float determinant() const;
	mat4 translate(const vec3& v) const;
	mat4 rotate(const vec3& v, float angle) const;
	mat4 scale(const vec3& v) const;
	bool decompose(vec3& Scale, quat& Orientation, vec3& Translation, vec3& Skew, vec4& Perspective) const;
};
mat4::mat4(const quat &q)
{
	mat4 Result;
	float qxx(q.x * q.x);
	float qyy(q.y * q.y);
	float qzz(q.z * q.z);
	float qxz(q.x * q.z);
	float qxy(q.x * q.y);
	float qyz(q.y * q.z);
	float qwx(q.w * q.x);
	float qwy(q.w * q.y);
	float qwz(q.w * q.z);

	Result[0][0] = 1 - 2 * (qyy +  qzz);
	Result[0][1] = 2 * (qxy + qwz);
	Result[0][2] = 2 * (qxz - qwy);

	Result[1][0] = 2 * (qxy - qwz);
	Result[1][1] = 1 - 2 * (qxx +  qzz);
	Result[1][2] = 2 * (qyz + qwx);

	Result[2][0] = 2 * (qxz + qwy);
	Result[2][1] = 2 * (qyz - qwx);
	Result[2][2] = 1 - 2 * (qxx +  qyy);

	*this = Result.transpose();
}
quat mat4::ToQuat() const
{
	const mat4& m = *this;
	quat q;
	q.w = sqrt(1.f + m[0][0] + m[1][1] + m[2][2]) / 2.f;
	float w4 = 4.f * q.w;
	q.x = (m[2][1] - m[1][2]) / w4;
	q.y = (m[0][2] - m[2][0]) / w4;
	q.z = (m[1][0] - m[0][1]) / w4;
	return q;
}
inline vec4 mat4::column(int i) const
{
	return vec4(row[0][i], row[1][i], row[2][i], row[3][i]);
}
inline mat4 mat4::operator+(const mat4& m) const
{
	mat4 r;
	for (int i = 0; i < MAT4_LOOP; i++)
	{
		r[i]=row[i]+m[i];
	}
	return r;
}
inline mat4 mat4::operator-(const mat4& m) const
{
	mat4 r;
	for (int i = 0; i < MAT4_LOOP; i++)
	{
		r[i]=row[i]-m[i];
	}
	return r;
}
inline mat4 mat4::operator*(const mat4& m) const
{
	mat4 r;
	for (int i = 0; i < MAT4_LOOP; i++)
	{
		for (int j = 0; j < MAT4_LOOP; j++)
		{
			r[i][j] = dot(row[i], m.column(j));
		}
	}
 	return r;
}
inline vec4 mat4::operator*(const vec4& v) const
{
	vec4 r;
	for (int i = 0; i < MAT4_LOOP; i++)
	{
		r[i] = dot(row[i], v);
	}
 	return r;
}
inline mat4 mat4::operator*(float f) const
{
	mat4 r;
	for (int i = 0; i < MAT4_LOOP; i++)
	{
		for (int j = 0; j < MAT4_LOOP; j++)
		{
			r[i][j] *= f;
		}
	}
 	return r;
}
inline mat4 mat4::operator/(const mat4& m) const
{
	return (*this) * m.inverse();
}
inline vec4& mat4::operator[](int i)
{
    return row[i];
}
inline vec4 const& mat4::operator[](int i) const
{
	return row[i];
}
inline ostream& operator<<(ostream& os, mat4 &m)
{
	for (int i = 0; i < MAT4_LOOP; i++)
	{
		os << m[i];
	}
	os << endl;
	return os;
}
inline mat4 mat4::transpose() const
{
	mat4 m;
	for (int i = 0; i < MAT4_LOOP; i++)
	{
		m[i] = column(i);
	}
	return m;
}
inline mat4 mat4::inverse() const
{
	mat4 m = transpose();
	float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

	float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

	float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

	float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

	float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

	float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	vec4 Fac0(Coef00, Coef00, Coef02, Coef03);
	vec4 Fac1(Coef04, Coef04, Coef06, Coef07);
	vec4 Fac2(Coef08, Coef08, Coef10, Coef11);
	vec4 Fac3(Coef12, Coef12, Coef14, Coef15);
	vec4 Fac4(Coef16, Coef16, Coef18, Coef19);
	vec4 Fac5(Coef20, Coef20, Coef22, Coef23);

	vec4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
	vec4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
	vec4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
	vec4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

	vec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	vec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	vec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	vec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	vec4 SignA(+1, -1, +1, -1);
	vec4 SignB(-1, +1, -1, +1);
	mat4 Inverse = {Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB};
	Inverse = Inverse.transpose();
	return Inverse;

	vec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	vec4 Dot0(m[0] * Row0);
	float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	float OneOverDeterminant = 1 / Dot1;

	return Inverse * OneOverDeterminant;
}
inline float mat4::determinant() const
{
	mat4 m = transpose();
	float SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

	vec4 DetCof(
		+ (m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02),
		- (m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04),
		+ (m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05),
		- (m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05));

	return
		m[0][0] * DetCof[0] + m[0][1] * DetCof[1] +
		m[0][2] * DetCof[2] + m[0][3] * DetCof[3];
}
inline mat4 mat4::translate(const vec3& v) const
{
	mat4 m = transpose();
	mat4 r = m;
	r[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
	return r.transpose();
}
inline mat4 mat4::rotate(const vec3& v, float angle) const
{
	glm::mat4 glm_m;
	mat4 m = transpose();
	memcpy(&glm_m, &m, sizeof(mat4));
	glm::vec3 glm_axis;
	memcpy(&glm_axis, &v, sizeof(vec3));
	glm_m = glm::rotate(glm_m, angle, glm_axis);
	memcpy(&m, &glm_m, sizeof(mat4));
	return m.transpose();
	// mat4 m = transpose();

	// float const a = angle;
	// float const c = cos(a);
	// float const s = sin(a);

	// vec3 axis = v.normalize();
	// vec3 temp = (1.0f - c) * axis;

	// mat4 Rotate;
	// Rotate[0][0] = c + temp[0] * axis[0];
	// Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
	// Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

	// Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
	// Rotate[1][1] = c + temp[1] * axis[1];
	// Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

	// Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
	// Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
	// Rotate[2][2] = c + temp[2] * axis[2];

	// mat4 Result;
	// Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
	// Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
	// Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
	// Result[3] = m[3];

	// return Result.transpose();	
}
inline mat4 mat4::scale(const vec3& v) const
{
	mat4 m = transpose();
	mat4 Result;
	Result[0] = m[0] * v[0];
	Result[1] = m[1] * v[1];
	Result[2] = m[2] * v[2];
	Result[3] = m[3];
	return Result.transpose();
}
// inline bool mat4::decompose(vec3& Scale, quat& Orientation, vec3& Translation, vec3& Skew, vec4& Perspective) const
// {
// 	mat4 LocalMatrix = transpose();

// 	// Normalize the matrix.
// 	if(abs(LocalMatrix[3][3]) < epsilon<float>())
// 		return false;

// 	for(int i = 0; i < 4; ++i)
// 		for(int j = 0; j < 4; ++j)
// 			LocalMatrix[i][j] /= LocalMatrix[3][3];

// 	// perspectiveMatrix is used to solve for perspective, but it also provides
// 	// an easy way to test for singularity of the upper 3x3 component.
// 	mat4 PerspectiveMatrix = LocalMatrix;

// 	for(int i = 0; i < 3; i++)
// 		PerspectiveMatrix[i][3] = 0;

// 	PerspectiveMatrix[3][3] = 1;

// 	/// TODO: Fixme!
// 	if(abs(PerspectiveMatrix.determinant()) < epsilon<float>())
// 		return false;

// 	// First, isolate perspective.  This is the messiest.
// 	if(
// 		abs(LocalMatrix[0][3]) >= epsilon<float>() ||
// 		abs(LocalMatrix[1][3]) >= epsilon<float>() ||
// 		abs(LocalMatrix[2][3]) >= epsilon<float>())
// 	{
// 		// rightHandSide is the right hand side of the equation.
// 		vec4 RightHandSide;
// 		RightHandSide[0] = LocalMatrix[0][3];
// 		RightHandSide[1] = LocalMatrix[1][3];
// 		RightHandSide[2] = LocalMatrix[2][3];
// 		RightHandSide[3] = LocalMatrix[3][3];

// 		// Solve the equation by inverting PerspectiveMatrix and multiplying
// 		// rightHandSide by the inverse.  (This is the easiest way, not
// 		// necessarily the best.)
// 		mat4 InversePerspectiveMatrix = PerspectiveMatrix.inverse();//   inverse(PerspectiveMatrix, inversePerspectiveMatrix);
// 		mat4 TransposedInversePerspectiveMatrix = InversePerspectiveMatrix.transpose();//   transposeMatrix4(inversePerspectiveMatrix, transposedInversePerspectiveMatrix);

// 		Perspective = TransposedInversePerspectiveMatrix * RightHandSide;
// 			//  v4MulPointByMatrix(rightHandSide, transposedInversePerspectiveMatrix, perspectivePoint);

// 			// Clear the perspective partition
// 			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = 0.f;
// 			LocalMatrix[3][3] = 1.f;
// 		}
// 		else
// 		{
// 			// No perspective.
// 			Perspective = vec4(0, 0, 0, 1);
// 		}

// 		// Next take care of translation (easy).
// 		Translation = vec3(LocalMatrix[3].x, LocalMatrix[3].y, LocalMatrix[3].z);
// 		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

// 		vec3 Row[3], Pdum3;

// 		// Now get scale and shear.
// 		for(int i = 0; i < 3; ++i)
// 			for(int j = 0; j < 3; ++j)
// 				Row[i][j] = LocalMatrix[i][j];

// 		// Compute X scale factor and normalize first row.
// 		Scale.x = Row[0].length();// v3Length(Row[0]);

// 		Row[0] = Row[0].normalize();

// 		// Compute XY shear factor and make 2nd row orthogonal to 1st.
// 		Skew.z = dot(Row[0], Row[1]);
// 		Row[1] = Row[1] + Row[0] * -Skew.z;

// 		// Now, compute Y scale and normalize 2nd row.
// 		Scale.y = Row[1].length();
// 		Row[1] = Row[1].normalize();
// 		Skew.z /= Scale.y;

// 		// Compute XZ and YZ shears, orthogonalize 3rd row.
// 		Skew.y = dot(Row[0], Row[2]);
// 		Row[2] = Row[2] + Row[0] * -Skew.y;
// 		Skew.x = dot(Row[1], Row[2]);
// 		Row[2] = Row[2] + Row[1] * -Skew.x;

// 		// Next, get Z scale and normalize 3rd row.
// 		Scale.z = Row[2].length();
// 		Row[2] = Row[2].normalize();
// 		Skew.y /= Scale.z;
// 		Skew.x /= Scale.z;

// 		// At this point, the matrix (in rows[]) is orthonormal.
// 		// Check for a coordinate system flip.  If the determinant
// 		// is -1, then negate the matrix and the scaling factors.
// 		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
// 		if(dot(Row[0], Pdum3) < 0)
// 		{
// 			for(int i = 0; i < 3; i++)
// 			{
// 				Scale[i] *= -1.f;
// 				Row[i] *= -1.f;
// 			}
// 		}

// 		// Now, get the rotations out, as described in the gem.

// 		// FIXME - Add the ability to return either quaternions (which are
// 		// easier to recompose with) or Euler angles (rx, ry, rz), which
// 		// are easier for authors to deal with. The latter will only be useful
// 		// when we fix https://bugs.webkit.org/show_bug.cgi?id=23799, so I
// 		// will leave the Euler angle code here for now.

// 		// ret.rotateY = asin(-Row[0][2]);
// 		// if (cos(ret.rotateY) != 0) {
// 		//     ret.rotateX = atan2(Row[1][2], Row[2][2]);
// 		//     ret.rotateZ = atan2(Row[0][1], Row[0][0]);
// 		// } else {
// 		//     ret.rotateX = atan2(-Row[2][0], Row[1][1]);
// 		//     ret.rotateZ = 0;
// 		// }

// 		int i, j, k = 0;
// 		float root, trace = Row[0].x + Row[1].y + Row[2].z;
// 		if(trace > 0.f)
// 		{
// 			root = sqrt(trace + 1.f);
// 			Orientation.w = 0.5f * root;
// 			root = 0.5f / root;
// 			Orientation.x = root * (Row[1].z - Row[2].y);
// 			Orientation.y = root * (Row[2].x - Row[0].z);
// 			Orientation.z = root * (Row[0].y - Row[1].x);
// 		} // End if > 0
// 		else
// 		{
// 			static int Next[3] = {1, 2, 0};
// 			i = 0;
// 			if(Row[1].y > Row[0].x) i = 1;
// 			if(Row[2].z > Row[i][i]) i = 2;
// 			j = Next[i];
// 			k = Next[j];

// #           ifdef GLM_FORCE_QUAT_DATA_XYZW
//                 int off = 0;
// #           else
//                 int off = 1;
// #           endif

// 			root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + 1.f);

// 			Orientation[i + off] = 0.5f * root;
// 			root = 0.5f / root;
// 			Orientation[j + off] = root * (Row[i][j] + Row[j][i]);
// 			Orientation[k + off] = root * (Row[i][k] + Row[k][i]);
// 			Orientation.w = root * (Row[j][k] - Row[k][j]);
// 		} // End if <= 0

// 		return true;
// }
inline bool mat4::decompose(vec3& Scale, quat& Orientation, vec3& Translation, vec3& Skew, vec4& Perspective) const
{
	mat4 M = transpose();
	glm::mat4 glm_M;
	memcpy(&glm_M, &M, sizeof(mat4));
	glm::vec3 glm_pos, glm_scal, glm_skew;	glm::vec4 glm_per;	glm::quat glm_rot;
	bool result = glm::decompose(glm_M, glm_scal, glm_rot, glm_pos, glm_skew, glm_per);
	memcpy(&Scale, &glm_scal, sizeof(vec3));
	CP_XYZW(Orientation, glm_rot);
	memcpy(&Translation, &glm_pos, sizeof(vec3));
	memcpy(&Skew, &glm_skew, sizeof(vec3));
	memcpy(&Perspective, &glm_per, sizeof(vec4));
	return result;
}
mat4 OrthographicProjection(float xmag, float ymag, float znear, float zfar)
{
	float r = xmag, t = ymag, n = znear, f = zfar;

	mat4 result = {
		{1/r, 0, 0, 0},
		{0, 1/t, 0, 0},
		{0, 0, 2/(n-f), (f+n)/(n-f)},
		{0, 0, 0, 1}
	};
	return result;
}
mat4 PerspectiveProjection(float yfov, float aspect, float znear, float zfar)
{
	float a = aspect, y = yfov, n = znear, f = zfar;

	mat4 result = {
		{1/(a*tan(.5f*y)), 0, 0, 0},
		{0,1/tan(.5f*y), 0, 0},
		{0, 0, -(f+n)/(n-f), -(2.f*f*n)/(n-f)},
		{0, 0, -1, 0}
	};
	return result;
}
mat4 TranslationProjection(const vec3& translation)
{
	mat4 m = {
		{1, 0, 0, translation.x},
		{0, 1, 0, translation.y},
		{0, 0, 1, translation.z},
		{0, 0, 0, 1},
	};
	return m;
}
mat4 RotationXProjection(float radianceAngle)
{
	float cosA = cos(radianceAngle);
	float sinA = sin(radianceAngle);
	mat4 m = {
		{1, 0, 0, 0},
		{0, cosA, -sinA, 0},
		{0, sinA, cosA, 0},
		{0, 0, 0, 1}
	};
	return m;
}
mat4 RotationYProjection(float radianceAngle)
{
	float cosA = cos(radianceAngle);
	float sinA = sin(radianceAngle);
	mat4 m = {
		{cosA, 0, sinA, 0},
		{0, 1, 0, 0},
		{-sinA, 0, cosA, 0},
		{0, 0, 0, 1}
	};
	return m;
}
mat4 RotationZProjection(float radianceAngle)
{
	float cosA = cos(radianceAngle);
	float sinA = sin(radianceAngle);
	mat4 m = {
		{cosA, -sinA, 0, 0},
		{sinA, cosA, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	return m;
}
mat4 ScaleProjection(const vec3& scale)
{
	mat4 m = {
		{scale.x, 0, 0, 0},
		{0, scale.y, 0, 0},
		{0, 0, scale.z, 0},
		{0, 0, 0, 1}
	};
	return m;
}
mat4 RightHandZUpToYUpProjection()
{
	mat4 m = {
		{1, 0, 0, 0},
		{0, 0, 1, 0},
		{0,-1, 0, 0},
		{0, 0, 0, 1}
	};
	return m;
}
#endif
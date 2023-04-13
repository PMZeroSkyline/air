
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
//using namespace glm;
#include "SDK/STL/STL.h"
//ostream &operator<<(ostream &os, const vec3 v)
//{
//	for (size_t i = 0; i < 3; i++)
//		os << v[i] << ",";
//	return os;
//}
//ostream &operator<<(ostream &os, const vec4 v)
//{
//	for (size_t i = 0; i < 4; i++)
//		os << (v[i] < 0.00001 ? 0 : v[i]) << ",";
//	return os;
//}
//ostream &operator<<(ostream &os, const quat v)
//{
//	for (size_t i = 0; i < 4; i++)
//		os << (v[i] < 0.00001 ? 0 : v[i]) << ",";
//	return os;
//}
//ostream &operator<<(ostream &os, const mat4 &m)
//{
//	for (int i = 0; i < 4; i++)
//		os << m[i] << endl;
//	return os;
//}
//quat EulerToQuat(float roll, float pitch, float yaw) // roll (x), pitch (Y), yaw (z)
//{
//	// https://en.wikipedia.org/wiki/Conversion_between_quats_and_Euler_angles
//
//    quat q;
//
//    float cr = cos(roll * 0.5);
//    float sr = sin(roll * 0.5);
//    float cp = cos(pitch * 0.5);
//    float sp = sin(pitch * 0.5);
//    float cy = cos(yaw * 0.5);
//    float sy = sin(yaw * 0.5);
//
//    q.w = cr * cp * cy + sr * sp * sy;
//    q.x = sr * cp * cy - cr * sp * sy;
//    q.y = cr * sp * cy + sr * cp * sy;
//    q.z = cr * cp * sy - sr * sp * cy;
//    return q;
//}
//quat EulerToQuat(const vec3 &v)
//{
//    return EulerToQuat(v.x, v.y, v.z);
//}
//
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


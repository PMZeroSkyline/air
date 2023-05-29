#ifndef SHAPE_CASTING
#define SHAPE_CASTING

#include "Physic/Shape/LineSegment.h"
#include "Physic/Shape/Capsule.h"
#include "Physic/Shape/Sphere.h"
#include "Physic/Shape/OBB.h"

float sdLineSegment(LineSegment* l, const vec3& p)
{
    vec3 ab = l->b - l->a;
    vec3 ap = p - l->a;
    float apab = dot(ap, ab);
    if (apab <= 0.f)
    {
        return distance(l->a, p);
    }

    vec3 ba = l->a - l->b;
    vec3 bp = p - l->b;
    float babp = dot(ba, bp);
    if (babp <= 0.f)
    {
        return distance(l->b, p);
    }

    float proj = apab / ab.length();
    vec3 c = l->a + ab.normalize() * proj;
    return distance(c, p);
}

float sdCapsule(Capsule* c, const vec3& p)
{
    LineSegment l = LineSegment(c->a, c->b);
    float d = sdLineSegment(&l, p);
    return d - c->r;
}

float sdSphere(Sphere* s, const vec3& p)
{
    return distance(p, s->c) - s->r;
}

float sdOBB(OBB* o, const vec3& p)
{
    vec4 l = mat4(o->worldTransform.rotation).inverse() * vec4((o->worldTransform.translation - p), 1.f);
	vec3 localPoint = vec3(l.x, l.y, l.z) + o->c;

    vec3 d = abs(localPoint) - (o->worldTransform.scaling * o->e);

	float inside = min(max(d.x, max(d.y, d.z)), 0.f);
	float outside = max(d, 0.f).length();

    return inside + outside;
}

bool IsIntersectOBBOBB(OBB* a, OBB* b)
{
    return false;
}
bool IsIntersectOBBSphere(OBB* o, Sphere* s)
{
    return false;
}
bool IsIntersectOBBCapsule(OBB* o,Capsule* c)
{
    float sdBeg = sdOBB(o, c->a) - c->r;
    float sdEnd = sdOBB(o, c->b) - c->r;

    if (sdBeg < 0.f || sdEnd < 0.f)
    {
        return true;
    }
    return false;
}
bool IsIntersectSphereSphere(Sphere* a, Sphere* b)
{
    return false;
}
bool IsIntersectSphereCapsule(Sphere* a, Capsule* b)
{
    return false;
}
bool IsIntersectCapsuleCapsule(Capsule* a, Capsule* b)
{
    return false;
}

#endif
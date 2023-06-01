#ifndef CASTING_H
#define CASTING_H

#include "Physic/Shape/LineSegment.h"
#include "Physic/Shape/Capsule.h"
#include "Physic/Shape/Sphere.h"
#include "Physic/Shape/OBB.h"
#include "Physic/Shape/Plane.h"
#include "SDK/STL/STL.h"

Sphere Transf(Sphere* s, const mat4& m)
{
    return Sphere(ToVec3(m * vec4(s->c, 1.f)), s->r);
}
OBB Transf(OBB* o, const mat4& m)
{
    mat4 w = m * o->t.ToMatrix();
    return OBB(w);
}
Capsule Transf(Capsule* c, const mat4& m)
{
    return Capsule(ToVec3(m * vec4(c->a, 1.f)), ToVec3(m * vec4(c->b, 1.f)), c->r);
}
Plane Transf(Plane* p, const mat4& m)
{
    return Plane(ToVec3(m * vec4(p->p, 1.f)), ToVec3(m * vec4(p->n, 0.f)));
}
// Distance Function
float SDF(Plane* a, const vec3& p)
{
    return dot((p - a->p), a->n);
}
float UDF(LineSegment* l, const vec3& p)
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
float SDF(Capsule* c, const vec3& p)
{
    LineSegment l = LineSegment(c->a, c->b);
    float d = UDF(&l, p);
    return d - c->r;
}
float SDF(Sphere* s, const vec3& p)
{
    return distance(p, s->c) - s->r;
}
float SDF(OBB* o, const vec3& p)
{
    vec4 l = mat4(o->t.rotation).inverse() * vec4((o->t.translation - p), 1.f);
	vec3 lp = vec3(l.x, l.y, l.z);

    vec3 d = abs(lp) - o->t.scaling;

	float inside = min(max(d.x, max(d.y, d.z)), 0.f);
	float outside = max(d, 0.f).length();

    return inside + outside;
}

// Intersect Test Function
bool IsIntersect(Sphere* s1, Sphere* s2)
{
    return (SDF(s1, s2->c) - s2->r) <= 0.f;
}
bool IsIntersect(Sphere* s, OBB* o)
{
    return (SDF(o, s->c) - s->r) <= 0.f;
}
bool IsIntersect(Capsule* c, OBB* o) // not exact
{
    float sdA = SDF(o, c->a);
    float sdB = SDF(o, c->b);
    float sd = min(sdA, sdB) - c->r;
    return sd <= 0.f;
}
bool IsIntersect(Sphere* s, Capsule* c) // not exact
{
    float sd = SDF(c, s->c) - c->r;
    return sd <= 0.f;
}
bool IsIntersect(Capsule* c1, Capsule* c2) // not exact
{
    float sdA = SDF(c1, c2->a);
    float sdB = SDF(c1, c2->b);
    float sd = min(sdA, sdB) - c2->r;
    return sd <= 0.f;
}
bool IsIntersect(Shape* a, const mat4& aMatrix, Shape* b, const mat4& bMatrix)
{
    if (IsType(a, Sphere))
    {
        if (IsType(b, OBB))
        {
            Sphere x = Transf((Sphere*)a, aMatrix);
            OBB y = Transf((OBB*)b, bMatrix);
            return IsIntersect(&x, &y);
        }
        if (IsType(b, Sphere))
        {
            Sphere x = Transf((Sphere*)a, aMatrix);
            Sphere y = Transf((Sphere*)b, bMatrix);
            return IsIntersect(&x, &y);
        }
        if (IsType(b, Capsule))
        {
            Sphere x = Transf((Sphere*)a, aMatrix);
            Capsule y = Transf((Capsule*)b, bMatrix);
            return IsIntersect(&x, &y);
        }
    }
    if (IsType(a, Capsule))
    {
        if (IsType(b, OBB))
        {
            Capsule x = Transf((Capsule*)a, aMatrix);
            OBB y = Transf((OBB*)b, bMatrix);
            return IsIntersect(&x, &y);
        }
        if (IsType(b, Sphere))
        {
            Capsule x = Transf((Capsule*)a, aMatrix);
            Sphere y = Transf((Sphere*)b, bMatrix);
            return IsIntersect(&y, &x);
        }
        if (IsType(b, Capsule))
        {
            Capsule x = Transf((Capsule*)a, aMatrix);
            Capsule y = Transf((Capsule*)b, bMatrix);
            return IsIntersect(&x, &y);
        }
    }
    return 0.f;
}
#endif
#ifndef CASTING_H
#define CASTING_H

#include "Physic/Shape/LineSegment.h"
#include "Physic/Shape/Capsule.h"
#include "Physic/Shape/Sphere.h"
#include "Physic/Shape/OBB.h"
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

// Point SDF
float SDF(LineSegment* l, const vec3& p)
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
    float d = SDF(&l, p);
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

// Shape SDF
float SDF(Sphere* a, Sphere* b)
{
    return (distance(a->c, b->c) - (a->r + b->r));
}
float SDF(Sphere* s, OBB* o)
{
    return (SDF(o, s->c) - s->r);
}
float SDF(Capsule* c, OBB* o) // not exact
{
    float sdA = SDF(o, c->a) - c->r;
    float sdB = SDF(o, c->b) - c->r;
    float sd = min(sdA, sdB);
    return sd;
}
float SDF(Sphere* s, Capsule* c) // not exact
{
    float sdA = SDF(s, c->a) - c->r;
    float sdB = SDF(s, c->b) - c->r;
    float sd = min(sdA, sdB);
    return sd;
}
float SDF(Capsule* a, Capsule* b) // not exact
{
    float sdA = SDF(a, b->a) - b->r;
    float sdB = SDF(a, b->b) - b->r;
    float sd = min(sdA, sdB);
    return sd;
}
float SDF(Shape* a, const mat4& aMatrix, Shape* b, const mat4& bMatrix)
{
    if (IsType(a, Sphere))
    {
        if (IsType(b, OBB))
        {
            Sphere x = Transf((Sphere*)a, aMatrix);
            OBB y = Transf((OBB*)b, bMatrix);
            return SDF(&x, &y);
        }
        if (IsType(b, Sphere))
        {
            Sphere x = Transf((Sphere*)a, aMatrix);
            Sphere y = Transf((Sphere*)b, bMatrix);
            return SDF(&x, &y);
        }
        if (IsType(b, Capsule))
        {
            Sphere x = Transf((Sphere*)a, aMatrix);
            Capsule y = Transf((Capsule*)b, bMatrix);
            return SDF(&x, &y);
        }
    }
    if (IsType(a, Capsule))
    {
        if (IsType(b, OBB))
        {
            Capsule x = Transf((Capsule*)a, aMatrix);
            OBB y = Transf((OBB*)b, bMatrix);
            return SDF(&x, &y);
        }
        if (IsType(b, Sphere))
        {
            Capsule x = Transf((Capsule*)a, aMatrix);
            Sphere y = Transf((Sphere*)b, bMatrix);
            return SDF(&y, &x);
        }
        if (IsType(b, Capsule))
        {
            Capsule x = Transf((Capsule*)a, aMatrix);
            Capsule y = Transf((Capsule*)b, bMatrix);
            return SDF(&x, &y);
        }
    }
    return 0.f;
}
#endif
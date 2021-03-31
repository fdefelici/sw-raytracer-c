#include "vec3.h"
#include "math.h"

vec3 vec3_new(float x, float y, float z) {
    vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vec3 vec3_zero() {
    return vec3_new(0.f, 0.f, 0.f);
}

float vec3_magn(vec3* v) {
    return sqrtf(vec3_magn2(v));
}

float vec3_magn2(vec3* v) {
    return vec3_dot(v, v);
}

vec3 vec3_norm(vec3* v) {
    vec3 n;
    float magn = vec3_magn(v);
    n.x = v->x / magn;
    n.y = v->y / magn;
    n.z = v->z / magn;
    return n;
}

float vec3_dot(vec3* v1, vec3* v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

vec3 vec3_sub(vec3* v1, vec3* v2) {
    vec3 r;
    r.x = v1->x - v2->x;
    r.y = v1->y - v2->y;
    r.z = v1->z - v2->z;
    return r;
}


vec3 vec3_sum(vec3* v1, vec3* v2) {
    vec3 r;
    r.x = v1->x + v2->x;
    r.y = v1->y + v2->y;
    r.z = v1->z + v2->z;
    return r;
}

vec3 vec3_mul_scal(vec3* v, float s) {
    vec3 r;
    r.x = v->x * s;
    r.y = v->y * s;
    r.z = v->z * s;
    return r;
}

//Reflect vector v around vector n (normalized vector)
vec3 vec3_refl(vec3* v, vec3* n) {
    //reflVect = vect − 2*dot(vect,norm)*norm
    float dot2 = 2.f * vec3_dot(v, n);
    vec3 dot2norm = vec3_mul_scal(n, dot2);
    vec3 sub = vec3_sub(v, &dot2norm);
    return sub;
}

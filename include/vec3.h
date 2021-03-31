#ifndef vec3_H
#define vec3_H


typedef struct {
    float x;
    float y;
    float z;
} vec3;

vec3 vec3_new(float x, float y, float z);
vec3 vec3_zero();

float vec3_magn(vec3* v);
float vec3_magn2(vec3* v);
vec3 vec3_norm(vec3* v);
vec3 vec3_sub(vec3* v1, vec3* v2);
float vec3_dot(vec3* v1, vec3* v2);

vec3 vec3_sum(vec3* v1, vec3* v2);
vec3 vec3_mul_scal(vec3* v, float s);
vec3 vec3_refl(vec3* v, vec3* r);

#endif //vec3_H
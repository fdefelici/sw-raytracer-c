#ifndef SPHERE_H
#define SPHERE_H
#include "vec3.h"
#include "color.h"

typedef struct {
    color albedo;
    float ambient_factor;
    color specular_color;
    float specular_shiness_factor;
    float reflect_factor; //[0.f, 1.f]
} material;


typedef struct {
    vec3 position;
    float radius;
    material material;
} sphere;

#endif;
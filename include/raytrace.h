#ifndef RAYTRACE_H
#define RAYTRACE_H
#include "vec3.h"
#include "color.h"
#include "sphere.h"
#include "scene.h"
#include "common.h"

typedef struct {
    vec3 origin;
    vec3 direction;
} ray;

typedef struct {
    color color;
    sphere* obj;
    float distance;
    vec3 point;
    vec3 normal;
} ray_hit;

enum raycast_strategy_enum { RAYCAST_FIRST, RAYCAST_BEST };
typedef unsigned char raycast_strategy;

color ray_trace(ray* ray, scene* scene, int depth);

boolean ray_cast(ray* ray, sphere** spheres, int sphere_count, ray_hit* hit_out, raycast_strategy strategy);

#endif RAYTRACE_H
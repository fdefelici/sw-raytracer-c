#ifndef SCENE_H
#define SCENE_H
#include "vec3.h"
#include "sphere.h"
#include "SDL.h"
#include "color.h"

typedef struct {
    int width;
    int height;
    float aspect_ratio;
} screen;

typedef struct {
    vec3 position;
    float fov; //full vertical fov
    float fov_tan; //tangent value calculated on half fov converted to radians
} camera;

typedef struct {
    screen* screen;
    camera* camera;
    sphere** sphere_list;
    int sphere_count;
    color background_color;
    vec3  light_direction;
    float  light_intensity;
    color light_color;
} scene;

scene* scene_create(int w, int h);
void scene_update(scene*, SDL_Renderer* renderer, float delta_time);
void scene_destroy(scene*);

#endif //SCENE_H
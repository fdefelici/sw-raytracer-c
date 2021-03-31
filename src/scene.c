#include "scene.h"
#include <stdlib.h>
//#define _USE_MATH_DEFINES //Per "vedere" le define come M_PI
//#include <math.h>
#include "raytrace.h"

scene* scene_create(int screen_width, int screen_height) {
    scene* s = malloc(sizeof(scene));

    s->screen = malloc(sizeof(screen));
    s->screen->width = screen_width;
    s->screen->height = screen_height;
    s->screen->aspect_ratio = (float)s->screen->width / (float)s->screen->height;

    s->camera = malloc(sizeof(camera));
    s->camera->position = vec3_new(0, 0, 0);
    s->camera->fov = 60.f; 
    //https://www.scratchapixel.com/images/upload/ray-tracing-camera/camprofile.png
    float half_fov = s->camera->fov / 2.f; //divido per 2 poiche' serve solo meta' del Fov verticale per il triangolo formato tra  [1 e 0] del range verticale [-1, 1]
    float fov_rad = half_fov * M_PI/180.f; //converte il fov da degree a radians
    s->camera->fov_tan = tanf(fov_rad);        

    s->sphere_count = 3;
    s->sphere_list = malloc(sizeof(void*) * s->sphere_count);

    s->sphere_list[0] = malloc(sizeof(sphere));
    s->sphere_list[0]->position = vec3_new(0, 0, -6.f);
    s->sphere_list[0]->radius = 1.f;
    s->sphere_list[0]->material.albedo = color_red();
    s->sphere_list[0]->material.ambient_factor = 0.3f;
    //s->sphere_list[0]->material.specular_color = color_black(); 
    //s->sphere_list[0]->material.specular_shiness_factor = 0.f; //no specular
    s->sphere_list[0]->material.reflect_factor = 0.f;
    
    s->sphere_list[1] = malloc(sizeof(sphere));
    s->sphere_list[1]->position = vec3_new(0.5f, 0, -3);
    s->sphere_list[1]->radius = 0.5f;
    s->sphere_list[1]->material.albedo = color_white();
    s->sphere_list[1]->material.ambient_factor = 0.f;
    s->sphere_list[1]->material.specular_color = color_white();
    s->sphere_list[1]->material.specular_shiness_factor = 60.f;
    s->sphere_list[1]->material.reflect_factor = 1.f;

    s->sphere_list[2] = malloc(sizeof(sphere));
    s->sphere_list[2]->position = vec3_new(-1.f, 0, -3.f);
    s->sphere_list[2]->radius = 0.5f;
    s->sphere_list[2]->material.albedo = color_green();
    s->sphere_list[2]->material.ambient_factor = 0.1f;
    //s->sphere_list[2]->material.specular_color = color_white();
    //s->sphere_list[2]->material.specular_shiness_factor = 0.f; //no specular
    s->sphere_list[2]->material.reflect_factor = 0.2f;
    
    s->background_color = color_black();

    //vec3 light_dir = vec3_new(-1.f, 0.f, -0.7f);
    //vec3 light_dir = vec3_new(0.f, -1.f, 0.f);
    vec3 light_dir = vec3_new(0.f, -1.f, -1.f);
    light_dir = vec3_norm(&light_dir);
    s->light_direction = light_dir;
    s->light_intensity = 1.f; 
    //s->light_color = color_new(0.8f, 0.5f, 0.4f);
    s->light_color = color_white();


    return s;
}

void scene_update(scene* s, SDL_Renderer* r, float delta_time) {
    int width = s->screen->width;
    int height = s->screen->height;
    float ratio  = s->screen->aspect_ratio;
    float fov_tan = s->camera->fov_tan;

    vec3 ray_origin = s->camera->position;

    for (size_t h = 0; h < height; ++h) { //rows
        for (size_t w = 0; w < width; ++w) { //cols

            float plane_x = (2.f * ((float)w + 0.5f)/(float)width) - 1.f;
            float plane_y = (1.f - 2.f * ((float)h + 0.5f)/(float)height);
            float plane_z = s->camera->position.z - 1.f; //Plan at distance of |1| from the camera z

            //adjust with fov and ratio
            plane_x = plane_x * fov_tan * ratio; 
            plane_y = plane_y * fov_tan;        
            
            vec3 point = vec3_new(plane_x, plane_y, plane_z);
            vec3 raydir = vec3_sub(&point, &ray_origin);
            raydir = vec3_norm(&raydir);

            ray ray;
            ray.origin = ray_origin;
            ray.direction = raydir;

            color color = ray_trace(&ray, s, 1);
            SDL_SetRenderDrawColor(r, 
                        (Uint8)(color.r * 255.f), 
                        (Uint8)(color.g * 255.f), 
                        (Uint8)(color.b * 255.f),
                        255); 
            SDL_RenderDrawPoint(r, w, h);
        }
    }

}
void scene_destroy(scene* s) {
    for(int i=0; i < s->sphere_count; i++) {
        free(s->sphere_list[i]);
    }
    free(s->sphere_list);
    free(s->camera);
    free(s->screen);

    free(s);

}
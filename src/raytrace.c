#include "raytrace.h"
#include "scene.h"
#include "common.h"
#include <float.h> 
#include <limits.h>

color color_mix(color* base, color* added, float factor) {
    color res;
    res.r = base->r * (1-factor) + added->r * factor;
    res.g = base->g * (1-factor) + added->g * factor;
    res.b = base->b * (1-factor) + added->b * factor;
    return res;
}

boolean sphere_intersect(sphere* sphere, ray* ray, ray_hit* hit_out) {
    vec3* center = &sphere->position;
    vec3* rayorig = &ray->origin;
    vec3* raydir = &ray->direction;
    float radius2 = sphere->radius * sphere->radius;
    
    vec3 l = vec3_sub(center, rayorig); 
    float tca = vec3_dot(&l, raydir);
    if (tca < 0) return false; 
    float d2 = vec3_magn2(&l) - tca * tca; 
    if (d2 > radius2) return false; 
    float thc = sqrt((double)radius2 - (double)d2); 
    float t0 = tca - thc; 
    float t1 = tca + thc; 

    if (t0 < t1) hit_out->distance = t0;
    else hit_out->distance = t1;

    //(P)oint = (O)rigin + t*(D)irection
    vec3 unnorm_dir = vec3_mul_scal(raydir, hit_out->distance);
    hit_out->point = vec3_sum(rayorig, &unnorm_dir);

    //Point Normal =  norm((P)oint - (C)enter)
    vec3 unnorm_normal = vec3_sub(&hit_out->point, center);
    hit_out->normal = vec3_norm(&unnorm_normal);

    hit_out->color = sphere->material.albedo;
    hit_out->obj = sphere;
    return true;
}

color ray_trace(ray* primary_ray, scene* scene, int depth) {
    if (depth > 3) return scene->background_color;

    #pragma region Calculate Primary Ray
    ray_hit hit;
    boolean has_hit = ray_cast(primary_ray, scene->sphere_list, scene->sphere_count, &hit, RAYCAST_BEST);
    if (!has_hit) return scene->background_color;
    #pragma endregion
    
    //Start Shading with Ambient
    material* obj_mat = &hit.obj->material;

    color ambient = color_mul_scal(&hit.color, obj_mat->ambient_factor);

    #pragma region Calculate Shadow Ray
    float bias = 1e-4; //  Move origin to avoid intersection with itself
    vec3 biased_normal = vec3_mul_scal(&hit.normal, bias);
    vec3 biased_point = vec3_sum(&hit.point, &biased_normal);
    
    ray shadow_ray;
    shadow_ray.origin = biased_point;
    vec3 inverted_light_dir = vec3_mul_scal(&scene->light_direction, -1.f);
    shadow_ray.direction = inverted_light_dir;

    ray_hit shadow_hit;
    boolean shadow_has_hit = ray_cast(&shadow_ray, scene->sphere_list, scene->sphere_count, &shadow_hit, RAYCAST_FIRST);
    if (shadow_has_hit) return ambient;
    #pragma endregion

    #pragma region Finalize Shading Blinn-Phong Model (diffuse + specular) 
    //diffuse: ObjectColor * Lambert * LightColor * LightIntensity
    float lambert = MAX(vec3_dot(&hit.normal, &shadow_ray.direction),  0);
    lambert *=  (1.f - obj_mat->reflect_factor);
    color diffuse_light = color_mul_scal(&scene->light_color, lambert * scene->light_intensity);
    color diffuse = color_mul(&hit.color, &diffuse_light);

    //specular: ObjectSpecularColor * SpecularCoeff * LightColor * LightIntensity 
    //Blinn 
    color specular = color_black(); //0;
    if (obj_mat->specular_shiness_factor > 0) {
        vec3 L = shadow_ray.direction;
        vec3 E = vec3_mul_scal(&hit.point, -1.f);
        vec3 H = vec3_sum(&L, &E);
        H = vec3_norm(&H);

        float specular_strength = MAX(vec3_dot(&hit.normal, &H), 0.f);
        float specular_intensity = powf(specular_strength, obj_mat->specular_shiness_factor);
        specular = color_mul_scal(&obj_mat->specular_color, specular_intensity);
        specular = color_mul(&specular, &scene->light_color);
    }

    color phong = color_black();
    phong = color_sum(&phong, &ambient);
    phong = color_sum(&phong, &diffuse);
    phong = color_sum(&phong, &specular);

    color pixel_color = phong;
    #pragma endregion 
    
    //Recurs on ray bounces in case material is reflective
    #pragma region Reflection
    vec3 reflVect = vec3_refl(&primary_ray->direction, &hit.normal);
    reflVect = vec3_norm(&reflVect);
    
    ray reflRay;
    reflRay.origin = biased_point;  //Move origin to avoid intersection with itself
    reflRay.direction = reflVect;

    color reflColor = ray_trace(&reflRay, scene, depth + 1); 
    reflColor = color_mul_scal(&reflColor, obj_mat->reflect_factor);
    pixel_color = color_sum(&pixel_color, &reflColor);
    

    pixel_color = color_clamp(&pixel_color);
    //pixel_color = color_gamma_correction(&pixel_color);
    #pragma endregion

    return pixel_color;
}

boolean ray_cast(ray* ray, sphere** spheres, int sphere_count, ray_hit* hit_out, raycast_strategy strategy) {
    ray_hit best_hit;
    best_hit.distance = FLT_MAX;
    boolean has_best_hit = false;
    for (size_t i = 0; i < sphere_count; i++) {
        ray_hit hit;
        boolean has_hit = sphere_intersect(spheres[i], ray, &hit);
        if (has_hit && hit.distance < best_hit.distance) {
            best_hit = hit;
            has_best_hit = true;
            if (strategy == RAYCAST_FIRST) break;
        }
    }

    *hit_out = best_hit;
    return has_best_hit;
}

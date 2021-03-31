#ifndef COLOR_H
#define COLOR_H
#include "common.h"
#include <math.h>

typedef struct {
    float r;
    float g;
    float b;
} color;

static color color_red() {
    color c;
    c.r = 1.f;
    c.g = 0;
    c.b = 0;
    return c;
}

static color color_black() {
    color c;
    c.r = 0;
    c.g = 0;
    c.b = 0;
    return c;
}

static color color_blue() {
    color c;
    c.r = 0;
    c.g = 0;
    c.b = 1.f;
    return c;
}

static color color_green() {
    color c;
    c.r = 0;
    c.g = 1.f;
    c.b = 0.f;
    return c;
}

static color color_white() {
    color c;
    c.r = 1.f;
    c.g = 1.f;
    c.b = 1.f;
    return c;
}

static color color_new(float r, float g, float b) {
    color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

static color color_mul_scal(color* c, float s) {
    color res;
    res.r = c->r * s;
    res.g = c->g * s;
    res.b = c->b * s;
    return res;
}

static color color_mul(color* c1, color* c2) {
    color res;
    res.r = c1->r * c2->r;
    res.g = c1->g * c2->g;
    res.b = c1->b * c2->b;
    return res;
}

static color color_sum(color* c1, color* c2) {
    color res;
    res.r = c1->r + c2->r;
    res.g = c1->g + c2->g;
    res.b = c1->b + c2->b;
    return res;
}

static color color_clamp(color* c) {
    color res;
    res.r = MIN(1.f, c->r);
    res.g = MIN(1.f, c->g);
    res.b = MIN(1.f, c->b);
    return res;
}

static color color_gamma_correction(color* c) {
    color res;
    res.r = powf(c->r, 1.f / 2.2f); 
    res.g = powf(c->g, 1.f / 2.2f);
    res.b = powf(c->b, 1.f / 2.2f);
    return res;
}
#endif
#ifndef COMMON_H
#define COMMON_H

enum boolean_enum { false = 0, true = 1 };
typedef unsigned char boolean;

#define MIN(a, b) (a < b) ? a : b
#define MAX(a, b) (a > b) ? a : b
#define CLAMP(a)  MAX(0.f, MIN(1.f, a))

#endif;
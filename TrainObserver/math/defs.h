#pragma once

#define BIG_FLOAT 3.40282e+038f
#define EPSILON  0.000000001f
#define PI 3.14159265f
#define RANDF (float(rand()) / float(RAND_MAX))
#define GRGB(r,g,b)	((unsigned int)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned int)(unsigned char)(b))<<16)))


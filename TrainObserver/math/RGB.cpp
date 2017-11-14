#include "RGB.h"
#include "Vector3.h"

#define saturate(v) (v > 1.0f ? 1.0f : v < 0.0f ? 0.0f : v)


graph::gRGB::gRGB(const Vector3& v):
	r(unsigned char(saturate(v.x) * 255.0f)),
	g(unsigned char(saturate(v.y) * 255.0f)),
	b(unsigned char(saturate(v.z) * 255.0f))
{

}

graph::gRGB::gRGB(unsigned int clr):
	r(unsigned char(clr>>24)),
	g(unsigned char(clr>>16)),
	b(unsigned char(clr>>8))
{
}

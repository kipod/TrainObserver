#pragma once

namespace graph
{
	struct Vector3;

	struct gRGB
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;

		gRGB(): r(0), g(0), b(0) {}
		gRGB(unsigned char r_, unsigned char g_, unsigned char b_): r(r_), g(g_), b(b_) {}
		gRGB(const Vector3& v);
		gRGB(unsigned int clr);

		inline bool operator==(const gRGB& rgb) const
		{
			return rgb.r == r && rgb.g == g && rgb.b == b;
		}
	};
}

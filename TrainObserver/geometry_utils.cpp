#include "geometry_utils.h"
#include "vertex_formats.h"
#include <sys/stat.h>

bool fileExists(const std::string& path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

void generateTangentAndBinormal(XYZNUVTB& v)
{
	static graph::Vector3 up1(0, 0, 1);
	static graph::Vector3 up2(0, 1, 0);

	auto& up = up1;
	if ((v.normal - up1).LengthSquared() < 0.01f)
	{
		up = up2;
	}

	v.tangent = v.normal * up;
	v.binormal = v.normal * v.tangent;

	v.tangent.Normalize();
	v.binormal.Normalize();
}

#pragma once
#include "math/vector3.h"

struct Post;
struct Train;

namespace SpaceUI
{
void createPostUI(const Vector3& pos, const Post& post);
void createTrainUI(const Vector3& pos, const Train& train);
} // namespace SpaceUI

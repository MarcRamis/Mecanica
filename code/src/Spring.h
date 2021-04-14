#pragma once

#include "Mesh.h"

class Spring
{
	Mesh m;

	int p1_idx, p2_idx;
	float k_e, k_d;
	float rest_distance;

	Spring();
};
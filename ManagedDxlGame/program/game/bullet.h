#pragma once
#include "../library/tnl_sequence.h"

class Bullet {
public:
	float rad_ = 10;
	tnl::Vector3 pos_;

	void updeat();
	void draw();
};
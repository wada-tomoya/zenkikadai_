#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../library/tnl_sequence.h"
#include "camera.h"

class Goal {
private:
	
	int graph_hdl = 0;

public:
	tnl::Vector3 pos_ = { 5420,1313,0 };
	const float width_ = 60.0f;
	const float height_ = 60.0f;

	Goal();
	void draw(const Camera& camera);
};
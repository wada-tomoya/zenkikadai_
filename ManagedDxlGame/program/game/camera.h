#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../library/tnl_sequence.h"

class Camera {
public:
	tnl::Vector3 target_ = {0,0,0};
	void update(tnl::Vector3 target_pos);
private:
	

};
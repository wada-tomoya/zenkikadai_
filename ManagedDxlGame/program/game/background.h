#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../library/tnl_sequence.h"
#include "camera.h"

class BackGround {
private:
	float graph_diameter = 2;	//�摜�̊g��{��
	int graph_size_w = 640;	//�摜�̌��̉���
	int graph_size_h = 480;	//�摜�̌��̏c��
	tnl::Vector3 pos_ = { 0,0,0 };
	int graph_ = 0;
public:
	BackGround();

	void update(const Player& player);
	void draw(const Camera& camera);
};

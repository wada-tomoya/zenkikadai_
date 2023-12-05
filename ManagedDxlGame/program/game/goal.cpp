#include "player.h"
#include "goal.h"

Goal::Goal() {
	graph_hdl = LoadGraph("graphics/goal.png");
}

void Goal::draw(const Camera& camera) {
	// �`��ʒu�̒����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�������Œ��S�Ɏʂ��悤�ɒ���
	tnl::Vector3 draw_pos = pos_ - camera.target_ + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawRotaGraph(draw_pos.x, draw_pos.y, 1.0f, 0, graph_hdl, true);
}
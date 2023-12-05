#include "player.h"
#include "background.h"

BackGround::BackGround() {
	graph_ = LoadGraph("graphics/background/pipo-bg001.jpg");
}

void BackGround::update(const Player& player) {

	pos_ = player.pos_ - tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
}

void BackGround::draw(const Camera& camera) {
	// 描画位置の調整　　　　　　　　　　　　　　　　　↓ここで中心に写すように調整
	tnl::Vector3 draw_pos = pos_ - camera.target_ + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + graph_size_w * graph_diameter + 1,draw_pos.y + graph_size_h * graph_diameter + 1, graph_, false);
	//DrawStringEx(10, 130, -1, "background");
}
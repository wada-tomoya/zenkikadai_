#include "enemy.h"

Enemy::Enemy(tnl::Vector3 pos) {
	hp_ = 10;	//体力

	speed_ = 0.5;//移動速度
	
	graph_ = LoadGraph("graphics/charachip/testenemy.png");	//画像
	
	//画像、当たり判定サイズ
	size_width = 32;
	size_height = 32;
	
	pos_ = pos;	//初期座標

	//ジャンプ用重力
	gravity_ = 0;
	//移動前座標
	prev_pos = 0;
}

void Enemy::updeat(float delta_time) {
	
		//ジャンプ用重力
	if (gravity_ <= gravity_max) {
		gravity_ += 0.98;
	}
		//重力付与
		pos_.y += gravity_;

		//移動シーケンス
		tnl_sequence_.update(delta_time);

}

void Enemy::draw(const Camera& camera) {
	// 描画位置の調整　　　　　　　　　　　　　　　　　↓ここで中心に写すように調整
	tnl::Vector3 draw_pos = pos_ - camera.target_ + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawRotaGraph(draw_pos.x, draw_pos.y, 1.0f, 0, graph_, true);
}

bool Enemy::move1(const float delta_time) {

	TNL_SEQ_CO_FRM_YIELD_RETURN(1, delta_time, [&]() {
		gravity_ = jumpPower_;
		});
	
	tnl_sequence_.change(&Enemy::move2);

	TNL_SEQ_CO_END;
}
bool Enemy::move2(const float delta_time) {

	TNL_SEQ_CO_TIM_YIELD_RETURN(movetime, delta_time, [&]() {
		pos_.x -= speed_;
		});

	tnl_sequence_.change(&Enemy::move3);

	TNL_SEQ_CO_END
}
bool Enemy::move3(const float delta_time) {

	TNL_SEQ_CO_FRM_YIELD_RETURN(1, delta_time, [&]() {
		gravity_ = jumpPower_;
		});

	tnl_sequence_.change(&Enemy::move4);

	TNL_SEQ_CO_END;
}
bool Enemy::move4(const float delta_time) {

	TNL_SEQ_CO_TIM_YIELD_RETURN(movetime, delta_time, [&]() {
		pos_.x += speed_;
		});
	 
	tnl_sequence_.change(&Enemy::move1);

	TNL_SEQ_CO_END
}

//bool Enemy::jump(const float delta_time) {
//
//
//}

void Enemy::death(float delta_time) {

}
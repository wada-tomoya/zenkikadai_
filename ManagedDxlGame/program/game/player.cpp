#include "player.h"
//#include "map.h"
//#include "gamemanager.h"



Player::Player() {
	//体力
	hp_ = 50;
	//画像読み込み
	//graph_ = LoadGraph("graphics/charachip/testplayer2.png");
	//プレイヤー座標
	pos_ = { 100,1700,0 };
	//移動速度
	speed_ = 5;
	//サイズ
	size_width = 32;
	size_height = 48;
	//ジャンプ用重力
	gravity_ = 0;	
	//移動前座標
	prev_pos = 0;	

	std::string files[DIR_MAX] = {
		"graphics/c1_anim_right.png",
		"graphics/c1_anim_left.png"
	};

	//歩いてる時のアニメーション画像読み込み
	for (int i = 0; i < DIR_MAX; ++i) {
		LoadDivGraph(files[i].c_str(), 4, 4, 1, 32, 48, anim_hdls[i]);
	}

	//攻撃エフェクトのアニメーション画像読み込み
	LoadDivGraph("graphics/effect/pipo-btleffect055.png", 10, 10, 1, 120, 120, attack_effect_graph);

	life_graph = LoadGraph("graphics/ito_gpc_/mygame/heart.png");
}

void Player::updeat(float delta_time) {

	//ジャンプ用重力
	if (gravity_ <= gravity_max) {
		gravity_ += 0.98f;
	}
	//重力付与
	pos_.y += gravity_;

	prev_pos = pos_;	//移動前座標

	/*line_pos = {pos_.x, pos_.y - (pos_.y - prev_pos.y)/2, 0};
	line_height = size_height + (pos_.y - prev_pos.y);
	line_prev_pos = { line_pos.x, line_pos.y + (pos_.y - prev_pos.y), 0 };*/

	if (damage_flag) {
		damage();
	}
	if (invincible_flag) {
		invincible(delta_time);
	}
	if (life_Decrement_flag) {
		life_Decrement();
	}
	move(delta_time);	//左右移動
	jump();	//ジャンプ

	
}

void Player::draw(float delta_time, const Camera& camera) {

	// 描画位置の調整　　　　　　　　　　　　　　　　　↓ここで中心に写すように調整
	tnl::Vector3 draw_pos = pos_ - camera.target_ + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	//プレイヤー表示
	if (flashing == true) {
		DrawRotaGraph(draw_pos.x, draw_pos.y, 1.0f, 0, anim_hdls[anim_dir][anim_frame], true);
	}

	life_draw();
	
	/*DrawStringEx(10, 50, -1, "player pos_x = %f nplayer pos_y = %f", pos_.x, pos_.y);
	DrawStringEx(10, 90, -1, "gravity = %f", gravity_);
	DrawStringEx(10, 110, -1, "player_hp = %d", hp_);
	DrawStringEx(10, 150, -1, "無敵時間 = %f", invincible_count);
	DrawStringEx(10, 170, -1, "点滅時間 = %f", flashing_count);
	DrawStringEx(10, 190, -1, "jumpcount = %d", jumpcount_);
	DrawStringEx(10, 210, -1, "向き,1が左0が右 = %d", direction_);
	DrawStringEx(10, 230, -1, "effect_pos_x = %f effect_pos_y = %f", effect_pos.x, effect_pos.y);
	DrawStringEx(10, 250, -1, "attack_anim_time = %f", attack_anim_time);
	DrawStringEx(10, 270, -1, "attack_anim_frame = %d",attack_anim_frame);
	DrawStringEx(10, 290, -1, "attack_flag = %d", attack_flag);
	DrawStringEx(10, 310, -1, "jump_count = %d", jumpcount_);*/

	attack(delta_time, camera);	//攻撃
}

//左右移動
void Player::move(float delta_time) {
	//右に移動
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		pos_.x += speed_;

		//アニメーション再生
		anim_time += delta_time;
		if (anim_time > 0.1f) {
			anim_frame++;
			anim_frame %= 4;
			anim_time = 0;
		}
		anim_dir = DIR_RIGHT;	//右向きのアニメーション

		direction_ = Right_;	//向きを右にする
	}
	//左に移動
	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		pos_.x -= speed_;

		//アニメーション再生
		anim_time += delta_time;
		if (anim_time > 0.1f) {
			anim_frame++;
			anim_frame %= 4;
			anim_time = 0;
		}
		anim_dir = DIR_LEFT;	//左向きのアニメーション

		direction_ = Left_;	//向きを左にする
	}
}

//ジャンプ
void Player::jump() {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) && jumpcount_ < jumpmax_) {
		gravity_ = jumpPower_;
		jumpcount_++;
	}
}

//ダメージを受けた時
void Player::damage() {
	hp_ -= 10;

	damage_flag = false;
}

//無敵
void Player::invincible(float delta_time) {
	
	invincible_count += delta_time;	//無敵状態のカウント

	if (invincible_count <= invincible_time) {
		//無敵、敵との当たり判定無視
		invincible_flag = true;

		//キャラクター点滅
		flashing_count += delta_time;
		if (flashing_count <= 0.25) {	
			flashing = false;
		}
		else if (flashing_count <= 0.5) {
			flashing = true;
		}
		else if (flashing_count <= 1) {
			flashing = false;
			flashing_count = 0;
		}
	}
		//invincible_timeを超えると無敵解除
	else if (invincible_count > invincible_time) {
		invincible_flag = false;
		invincible_count = 0;
		flashing = true;
	}
}

//攻撃
void Player::attack(float delta_time, const Camera& camera) {
	//カメラの 描画位置の調整　　　　　　　　　　　　　　　　　↓ここで中心に写すように調整
	tnl::Vector3 draw_pos = effect_pos - camera.target_ + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	//右側に出る攻撃
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE) && attack_flag == false && direction_ == Right_) {
		attack_flag = true;
		effect_pos.x = pos_.x + (size_width / 2 + effect_size_width / 2);
		effect_pos.y = pos_.y;
	}

		if (attack_flag) {
			attack_anim_time += delta_time;
			if (attack_anim_time > 0.1f) {
				attack_anim_frame++;
				attack_anim_frame %= 10;
				attack_anim_time = 0;
			}
			DrawRotaGraph(draw_pos.x, draw_pos.y, 0.25f, 0, attack_effect_graph[attack_anim_frame], true);	//エフェクト表示
			//DrawBoxEx(effect_pos, effect_size_width, effect_size_height, false);

			if (attack_anim_frame == 9) {
				attack_flag = false;
				attack_anim_frame = 0;
			}
		}
	
	//左側に出る攻撃
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE) && attack_flag == false && direction_ == Left_) {
		attack_flag = true;
		effect_pos.x = pos_.x - (size_width / 2 + effect_size_width / 2);
		effect_pos.y = pos_.y;
	}
	if (attack_flag) {
		attack_anim_time += delta_time;
		if (attack_anim_time > 0.1f) {
			attack_anim_frame++;
			attack_anim_frame %= 10;
			attack_anim_time = 0;
		}
		DrawRotaGraph(draw_pos.x, draw_pos.y, 0.25f, 0, attack_effect_graph[attack_anim_frame], true);	//エフェクト表示

		if (attack_anim_frame == 10) {
			attack_flag = false;
			attack_anim_frame = 0;
		}
	}
}

float Player::get_player_jumpPower_() {
		return gravity_ = enemy_stepon_jumpPower_;
}

void Player::life_draw() {
	DrawRotaGraph(life_pos.x, life_pos.y, 2.0f, 0, life_graph, true);
	DrawStringEx(life_pos.x + (life_graphsize / 2)+10 , life_pos.y-20, -1, "×　%d", life_);

}

void Player::life_Decrement() {
	if (life_ > 0) {
		life_--;
	}

	life_Decrement_flag = false;
}

void Player::get_life(int player_life) {
	player_life = life_;
}
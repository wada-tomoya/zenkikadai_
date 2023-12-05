#include <list>
#include "../dxlib_ext/dxlib_ext.h"
#include "scene_play.h"
#include "scene_result.h"
#include "gamemanager.h"

//Scene_Playインクリメント
Scene_Play::Scene_Play() {
	
	background_ = new BackGround;	//背景呼び出し
	player_ = new Player;	//プレイヤー呼び出し
	enemy_ = new Enemy(tnl::Vector3(100, 100, 0));	//エネミー呼び出し
	map_ = new Map;	//マップ呼び出し
	camera_ = new Camera; //カメラ呼び出し
	goal_ = new Goal;	//ゴール呼び出し

	//player_->get_life(player_life);

	//エネミーを追加
	enemys.emplace_back(new Enemy(tnl::Vector3(300, 1700, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(600, 1700, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(900, 1500, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(1200, 1570, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(1500, 1570, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(1800, 1700, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(2420, 1700, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(2970, 1730, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(3300, 1600, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(2700, 1700, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(4420, 1440, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(5000, 1700, 0)));
	enemys.emplace_back(new Enemy(tnl::Vector3(5380, 1700, 0)));
	//for (int i = 0; i <  15; i++) {
	//	enemys.emplace_back(new Enemy(tnl::Vector3(300 + (300 * i), 1100, 0)));		/*tnl::Vector3(100 + (100 * i), 100, 0)*/	/* enemy_->init_pos[i]*/
	//}

	//csvデータの1つの場所に当たり判定ブロックのデータを追加
	for (int i = 0; i < map_->csv_int_data.size(); ++i) {
		for (int k = 0; k < map_->csv_int_data[i].size(); ++k) {
			if (0 == map_->csv_int_data[i][k]) {
				continue;
			}
			tnl::Vector3 pos;
			pos.x = { 16.0f + k * map_->mapchip_size };
			pos.y = { 16.0f + i * map_->mapchip_size };
			blocks.emplace_back(new Block(pos));
		}
	}
}

//Scene_Playデクリメント
Scene_Play::~Scene_Play() {
	if (player_) {
		delete player_;	//プレイヤー開放
		player_ = nullptr;
	}

	if (enemy_) {
		delete enemy_;	//エネミーの開放
		enemy_ = nullptr;
	}
	for (auto it = enemys.begin(); it != enemys.end(); it++) {
		delete *it;
	}
	enemys.clear();

	if (map_) {
		delete map_;	//マップ開放
		map_ = nullptr;
	}
	
	if (background_) {
		delete background_;	//背景開放
		background_ = nullptr;
	}

	if (camera_) {	//カメラ開放
		delete camera_;
		camera_ = nullptr;
	}

	if (goal_) {
		delete goal_;
		goal_ = nullptr;
	}
}

void Scene_Play::draw(float delta_time) {
	background_->draw(*camera_);	//背景表示
	map_->draw(*camera_);	//マップ表示

	goal_->draw(*camera_);	//ゴール表示

	//DrawStringEx(10, 10, -1, "プレイ画面", true);

	player_->draw(delta_time, *camera_);	//プレイヤー表示（draw実行）
	
	// if(enemy_)enemy_->draw(*camera_);	//エネミー表示

	for (auto it = enemys.begin(); it != enemys.end(); it++) {
		(*it)->draw(*camera_);
	}

	//DrawStringEx(200, 10, -1, "hitcheck_sensor = %d", player_enemy_hitcheck);
	//DrawStringEx(200, 30, -1, "player_block_hitcheck = %d", player_block_hitcheck);

		
}

void Scene_Play::update(float delta_time) {
	
	sequence_.update(delta_time);	//sequence_のアップデート(実行)

	player_->updeat(delta_time);	//プレイヤーのupdeat実行

	
	// if(enemy_)enemy_->updeat(delta_time);


	for (auto it = enemys.begin(); it != enemys.end(); it++) {
		(*it)->updeat(delta_time);
	}

	camera_->update(player_->pos_);

	if (background_) background_->update(*player_);

	hitchack(); 
}

bool Scene_Play::seqIdle(float delta_time) {

//ゴールに着いたらシーン切り替え
	if (tnl::IsIntersectRect(
		player_->pos_.x,
		player_->pos_.y,
		player_->size_width,
		player_->size_height,
		goal_->pos_.x,
		goal_->pos_.y,
		goal_->width_,
		goal_->height_) ) {
		//シーン切り替え
		GameManager* mgr = GameManager::GetInstance();
		mgr->changescene(new (Scene_Rrsult));
	}
	/*else if (player_life <= 0) {
		GameManager* mgr = GameManager::GetInstance();
		mgr->changescene(new (Scene_Rrsult));
	}*/

	//エンターを押したらシーン切り替え
	/*if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		GameManager* mgr = GameManager::GetInstance();
		mgr->changescene(new (Scene_Rrsult));
	}*/
	return true;
}

void Scene_Play::hitchack() {

	//プレイヤーに近い順にソート
	blocks.sort([this](Block* l, Block* r) {
		float ld = (player_->pos_ - l->pos_).length();
		float rd = (player_->pos_ - r->pos_).length();
		return ld < rd;
		});

	//ブロックとプレイヤーの衝突判定＆補正
	for (auto blk : blocks)
	{
		player_block_hitcheck = tnl::IsIntersectRectToCorrectPosition(
			player_->pos_,
			player_->prev_pos,
			player_->size_width,
			player_->size_height,
			blk->pos_,
			map_->mapchip_size,
			map_->mapchip_size); 
		switch (player_block_hitcheck)
		{
		case 1:
		case 2:
			player_block_hitcheck = 0;

		case 3:
			//player_->gravity_ = 0;	//重力リセット
			player_->jumpcount_ = 0;	//ジャンプカウントリセット

		case 4:
			player_block_hitcheck = 0;
			break;

		default:
			break;
		}
			
		

	}
	//if (player_block_hitcheck == 1 || player_block_hitcheck == 2) {
	//	player_block_hitcheck = 0;
	//}
	//if (player_block_hitcheck == 3) {
	//	player_->gravity_ = 0;	//重力リセット
	//
	//}

	if (enemy_) {
		//エネミーとブロックの衝突判定＆補正
		for (auto blk : blocks)
		{
			for (auto enemy : enemys) {
				if (tnl::IsIntersectRectToCorrectPosition(
					enemy->pos_,
					enemy->prev_pos,
					enemy->size_width,
					enemy->size_height,
					blk->pos_,
					map_->mapchip_size,
					map_->mapchip_size)) {
					//enemy->gravity_ = 0;	//敵の重力リセット
				}
			}
		}

		//プレイヤーとエネミーの衝突判定＆補正
		if (player_->invincible_flag == false) {	//無敵状態フラグがfalseなら実行
			for (auto it = enemys.begin(); it != enemys.end(); ) {
				player_enemy_hitcheck = tnl::IsIntersectRectToCorrectPosition(
					player_->pos_,
					player_->prev_pos,
					player_->size_width,
					player_->size_height,
					(*it)->pos_,
					(*it)->size_width,
					(*it)->size_height);
				switch (player_enemy_hitcheck)
				{
					case 1:
					case 2:
					case 4:
						player_->life_Decrement_flag = true;
						player_->damage_flag = true;	//ダメージフラグtrue
						player_->invincible_flag = true;	//無敵フラグtrue
						
						break;

					case 3:
						player_->get_player_jumpPower_();

						delete (*it);
						it = enemys.erase(it);
						continue;
						//break;

					default:
						break;
				}
				it++;
			}
		}

		//プレイヤーの攻撃と敵が当たった時
		for (std::list<Enemy*>::iterator it = enemys.begin(); it != enemys.end();) {
			if (tnl::IsIntersectRect(
				player_->effect_pos.x,
				player_->effect_pos.y,
				player_->effect_size_width,
				player_->effect_size_height,
				(*it)->pos_.x,
				(*it)->pos_.y,
				(*it)->size_width,
				(*it)->size_height))
			{
				delete* it;	//エネミーの開放
				it = enemys.erase(it);
				continue;
			}
			it++;
		}
	}

	//プレイヤーと敵が当たった時の処理
	//if (player_enemy_hitcheck == 1 || player_enemy_hitcheck == 2) {
	//	player_enemy_hitcheck = 0;
	//	player_->damage_flag = true;	//ダメージフラグtrue
	//	player_->invincible_flag = true;	//無敵フラグtrue
	//}
	//for (auto it = enemys.begin(); it != enemys.end();){
	//	if (player_enemy_hitcheck == 3) {
	//		
	//		player_->get_player_jumpPower_();

	//		delete (*it);
	//		it = enemys.erase(it);
	//		continue;
	//	}
	//	it++;
	//}
	//if (player_enemy_hitcheck != 0) {
	//	player_enemy_hitcheck = 0;
	//}



	//プレイヤーとエネミーの衝突判定＆補正２
	/*bool IsIntersectRect(const int a_rect_x, const int a_rect_y, const int a_rect_size_w, const int a_rect_size_h,
		const int b_rect_x, const int b_rect_y, const int b_rect_size_w, const int b_rect_size_h);*/

	//プレイヤーとエネミーの衝突判定＆補正３
	/*bool IsIntersectRectPrimitive(const int a_left, const int a_right, const int a_top, const int a_bottom,
		const int b_left, const int b_right, const int b_top, const int b_bottom); */
}

#include <list>
#include "../dxlib_ext/dxlib_ext.h"
#include "scene_play.h"
#include "scene_result.h"
#include "gamemanager.h"

//Scene_Play�C���N�������g
Scene_Play::Scene_Play() {
	
	background_ = new BackGround;	//�w�i�Ăяo��
	player_ = new Player;	//�v���C���[�Ăяo��
	enemy_ = new Enemy(tnl::Vector3(100, 100, 0));	//�G�l�~�[�Ăяo��
	map_ = new Map;	//�}�b�v�Ăяo��
	camera_ = new Camera; //�J�����Ăяo��
	goal_ = new Goal;	//�S�[���Ăяo��

	//player_->get_life(player_life);

	//�G�l�~�[��ǉ�
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

	//csv�f�[�^��1�̏ꏊ�ɓ����蔻��u���b�N�̃f�[�^��ǉ�
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

//Scene_Play�f�N�������g
Scene_Play::~Scene_Play() {
	if (player_) {
		delete player_;	//�v���C���[�J��
		player_ = nullptr;
	}

	if (enemy_) {
		delete enemy_;	//�G�l�~�[�̊J��
		enemy_ = nullptr;
	}
	for (auto it = enemys.begin(); it != enemys.end(); it++) {
		delete *it;
	}
	enemys.clear();

	if (map_) {
		delete map_;	//�}�b�v�J��
		map_ = nullptr;
	}
	
	if (background_) {
		delete background_;	//�w�i�J��
		background_ = nullptr;
	}

	if (camera_) {	//�J�����J��
		delete camera_;
		camera_ = nullptr;
	}

	if (goal_) {
		delete goal_;
		goal_ = nullptr;
	}
}

void Scene_Play::draw(float delta_time) {
	background_->draw(*camera_);	//�w�i�\��
	map_->draw(*camera_);	//�}�b�v�\��

	goal_->draw(*camera_);	//�S�[���\��

	//DrawStringEx(10, 10, -1, "�v���C���", true);

	player_->draw(delta_time, *camera_);	//�v���C���[�\���idraw���s�j
	
	// if(enemy_)enemy_->draw(*camera_);	//�G�l�~�[�\��

	for (auto it = enemys.begin(); it != enemys.end(); it++) {
		(*it)->draw(*camera_);
	}

	//DrawStringEx(200, 10, -1, "hitcheck_sensor = %d", player_enemy_hitcheck);
	//DrawStringEx(200, 30, -1, "player_block_hitcheck = %d", player_block_hitcheck);

		
}

void Scene_Play::update(float delta_time) {
	
	sequence_.update(delta_time);	//sequence_�̃A�b�v�f�[�g(���s)

	player_->updeat(delta_time);	//�v���C���[��updeat���s

	
	// if(enemy_)enemy_->updeat(delta_time);


	for (auto it = enemys.begin(); it != enemys.end(); it++) {
		(*it)->updeat(delta_time);
	}

	camera_->update(player_->pos_);

	if (background_) background_->update(*player_);

	hitchack(); 
}

bool Scene_Play::seqIdle(float delta_time) {

//�S�[���ɒ�������V�[���؂�ւ�
	if (tnl::IsIntersectRect(
		player_->pos_.x,
		player_->pos_.y,
		player_->size_width,
		player_->size_height,
		goal_->pos_.x,
		goal_->pos_.y,
		goal_->width_,
		goal_->height_) ) {
		//�V�[���؂�ւ�
		GameManager* mgr = GameManager::GetInstance();
		mgr->changescene(new (Scene_Rrsult));
	}
	/*else if (player_life <= 0) {
		GameManager* mgr = GameManager::GetInstance();
		mgr->changescene(new (Scene_Rrsult));
	}*/

	//�G���^�[����������V�[���؂�ւ�
	/*if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		GameManager* mgr = GameManager::GetInstance();
		mgr->changescene(new (Scene_Rrsult));
	}*/
	return true;
}

void Scene_Play::hitchack() {

	//�v���C���[�ɋ߂����Ƀ\�[�g
	blocks.sort([this](Block* l, Block* r) {
		float ld = (player_->pos_ - l->pos_).length();
		float rd = (player_->pos_ - r->pos_).length();
		return ld < rd;
		});

	//�u���b�N�ƃv���C���[�̏Փ˔��聕�␳
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
			//player_->gravity_ = 0;	//�d�̓��Z�b�g
			player_->jumpcount_ = 0;	//�W�����v�J�E���g���Z�b�g

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
	//	player_->gravity_ = 0;	//�d�̓��Z�b�g
	//
	//}

	if (enemy_) {
		//�G�l�~�[�ƃu���b�N�̏Փ˔��聕�␳
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
					//enemy->gravity_ = 0;	//�G�̏d�̓��Z�b�g
				}
			}
		}

		//�v���C���[�ƃG�l�~�[�̏Փ˔��聕�␳
		if (player_->invincible_flag == false) {	//���G��ԃt���O��false�Ȃ���s
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
						player_->damage_flag = true;	//�_���[�W�t���Otrue
						player_->invincible_flag = true;	//���G�t���Otrue
						
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

		//�v���C���[�̍U���ƓG������������
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
				delete* it;	//�G�l�~�[�̊J��
				it = enemys.erase(it);
				continue;
			}
			it++;
		}
	}

	//�v���C���[�ƓG�������������̏���
	//if (player_enemy_hitcheck == 1 || player_enemy_hitcheck == 2) {
	//	player_enemy_hitcheck = 0;
	//	player_->damage_flag = true;	//�_���[�W�t���Otrue
	//	player_->invincible_flag = true;	//���G�t���Otrue
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



	//�v���C���[�ƃG�l�~�[�̏Փ˔��聕�␳�Q
	/*bool IsIntersectRect(const int a_rect_x, const int a_rect_y, const int a_rect_size_w, const int a_rect_size_h,
		const int b_rect_x, const int b_rect_y, const int b_rect_size_w, const int b_rect_size_h);*/

	//�v���C���[�ƃG�l�~�[�̏Փ˔��聕�␳�R
	/*bool IsIntersectRectPrimitive(const int a_left, const int a_right, const int a_top, const int a_bottom,
		const int b_left, const int b_right, const int b_top, const int b_bottom); */
}

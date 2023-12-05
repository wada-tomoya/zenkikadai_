#include "player.h"
//#include "map.h"
//#include "gamemanager.h"



Player::Player() {
	//�̗�
	hp_ = 50;
	//�摜�ǂݍ���
	//graph_ = LoadGraph("graphics/charachip/testplayer2.png");
	//�v���C���[���W
	pos_ = { 100,1700,0 };
	//�ړ����x
	speed_ = 5;
	//�T�C�Y
	size_width = 32;
	size_height = 48;
	//�W�����v�p�d��
	gravity_ = 0;	
	//�ړ��O���W
	prev_pos = 0;	

	std::string files[DIR_MAX] = {
		"graphics/c1_anim_right.png",
		"graphics/c1_anim_left.png"
	};

	//�����Ă鎞�̃A�j���[�V�����摜�ǂݍ���
	for (int i = 0; i < DIR_MAX; ++i) {
		LoadDivGraph(files[i].c_str(), 4, 4, 1, 32, 48, anim_hdls[i]);
	}

	//�U���G�t�F�N�g�̃A�j���[�V�����摜�ǂݍ���
	LoadDivGraph("graphics/effect/pipo-btleffect055.png", 10, 10, 1, 120, 120, attack_effect_graph);

	life_graph = LoadGraph("graphics/ito_gpc_/mygame/heart.png");
}

void Player::updeat(float delta_time) {

	//�W�����v�p�d��
	if (gravity_ <= gravity_max) {
		gravity_ += 0.98f;
	}
	//�d�͕t�^
	pos_.y += gravity_;

	prev_pos = pos_;	//�ړ��O���W

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
	move(delta_time);	//���E�ړ�
	jump();	//�W�����v

	
}

void Player::draw(float delta_time, const Camera& camera) {

	// �`��ʒu�̒����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�������Œ��S�Ɏʂ��悤�ɒ���
	tnl::Vector3 draw_pos = pos_ - camera.target_ + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	//�v���C���[�\��
	if (flashing == true) {
		DrawRotaGraph(draw_pos.x, draw_pos.y, 1.0f, 0, anim_hdls[anim_dir][anim_frame], true);
	}

	life_draw();
	
	/*DrawStringEx(10, 50, -1, "player pos_x = %f nplayer pos_y = %f", pos_.x, pos_.y);
	DrawStringEx(10, 90, -1, "gravity = %f", gravity_);
	DrawStringEx(10, 110, -1, "player_hp = %d", hp_);
	DrawStringEx(10, 150, -1, "���G���� = %f", invincible_count);
	DrawStringEx(10, 170, -1, "�_�Ŏ��� = %f", flashing_count);
	DrawStringEx(10, 190, -1, "jumpcount = %d", jumpcount_);
	DrawStringEx(10, 210, -1, "����,1����0���E = %d", direction_);
	DrawStringEx(10, 230, -1, "effect_pos_x = %f effect_pos_y = %f", effect_pos.x, effect_pos.y);
	DrawStringEx(10, 250, -1, "attack_anim_time = %f", attack_anim_time);
	DrawStringEx(10, 270, -1, "attack_anim_frame = %d",attack_anim_frame);
	DrawStringEx(10, 290, -1, "attack_flag = %d", attack_flag);
	DrawStringEx(10, 310, -1, "jump_count = %d", jumpcount_);*/

	attack(delta_time, camera);	//�U��
}

//���E�ړ�
void Player::move(float delta_time) {
	//�E�Ɉړ�
	if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
		pos_.x += speed_;

		//�A�j���[�V�����Đ�
		anim_time += delta_time;
		if (anim_time > 0.1f) {
			anim_frame++;
			anim_frame %= 4;
			anim_time = 0;
		}
		anim_dir = DIR_RIGHT;	//�E�����̃A�j���[�V����

		direction_ = Right_;	//�������E�ɂ���
	}
	//���Ɉړ�
	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		pos_.x -= speed_;

		//�A�j���[�V�����Đ�
		anim_time += delta_time;
		if (anim_time > 0.1f) {
			anim_frame++;
			anim_frame %= 4;
			anim_time = 0;
		}
		anim_dir = DIR_LEFT;	//�������̃A�j���[�V����

		direction_ = Left_;	//���������ɂ���
	}
}

//�W�����v
void Player::jump() {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) && jumpcount_ < jumpmax_) {
		gravity_ = jumpPower_;
		jumpcount_++;
	}
}

//�_���[�W���󂯂���
void Player::damage() {
	hp_ -= 10;

	damage_flag = false;
}

//���G
void Player::invincible(float delta_time) {
	
	invincible_count += delta_time;	//���G��Ԃ̃J�E���g

	if (invincible_count <= invincible_time) {
		//���G�A�G�Ƃ̓����蔻�薳��
		invincible_flag = true;

		//�L�����N�^�[�_��
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
		//invincible_time�𒴂���Ɩ��G����
	else if (invincible_count > invincible_time) {
		invincible_flag = false;
		invincible_count = 0;
		flashing = true;
	}
}

//�U��
void Player::attack(float delta_time, const Camera& camera) {
	//�J������ �`��ʒu�̒����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�������Œ��S�Ɏʂ��悤�ɒ���
	tnl::Vector3 draw_pos = effect_pos - camera.target_ + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	//�E���ɏo��U��
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
			DrawRotaGraph(draw_pos.x, draw_pos.y, 0.25f, 0, attack_effect_graph[attack_anim_frame], true);	//�G�t�F�N�g�\��
			//DrawBoxEx(effect_pos, effect_size_width, effect_size_height, false);

			if (attack_anim_frame == 9) {
				attack_flag = false;
				attack_anim_frame = 0;
			}
		}
	
	//�����ɏo��U��
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
		DrawRotaGraph(draw_pos.x, draw_pos.y, 0.25f, 0, attack_effect_graph[attack_anim_frame], true);	//�G�t�F�N�g�\��

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
	DrawStringEx(life_pos.x + (life_graphsize / 2)+10 , life_pos.y-20, -1, "�~�@%d", life_);

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
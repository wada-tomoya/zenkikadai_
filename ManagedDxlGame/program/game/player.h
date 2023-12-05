#pragma once
#include "character.h"
#include "camera.h"



enum {
	DIR_RIGHT,
	DIR_LEFT,
	DIR_MAX
};

enum direction {
	Right_,
	Left_
};

//struct ATTACK {
//	int attack_effect_graph[10];	//�U���G�t�F�N�g�̉摜�n���h��
//	bool attack_flag = false;
//	int attack_anim_frame = 0;
//	float attack_anim_time = 0;
//	const int effect_size_width = 30;
//	const int effect_size_height = 30;
//	tnl::Vector3 effect_pos = { 0,0,0 };	//�U���G�t�F�N�g�̍��W
//};

class Player : public Character_Base {
private:
	int life_ = 3;
	int life_graph = 0;
	int life_graphsize = 32;
	tnl::Vector3 life_pos = { 50,100,0 };

	float jumpPower_ = -15.0f;
	int jumpmax_ = 1;	//�W�����v��max
	float gravity_max = 10.0f;
	float enemy_stepon_jumpPower_ = -10.0f;

	bool turnflag = false;	//���E�̐U������Atrue�͍�false�͉E

	bool flashing = true;	//�L�����N�^�[�̓_��
	float flashing_count = 0;	//�_�ł̃J�E���g

	float invincible_count = 0;	//���G���Ԃ̃J�E���g
	float invincible_time = 2;	//���G����

	float anim_time = 0;
	int anim_dir = DIR_RIGHT;	//�Đ�����A�j���[�V����
	int anim_frame = 0;			//�A�j���[�V�����̃t���[��
	int anim_hdls[DIR_MAX][4];

	int direction_ = Right_;	//�L�����N�^�[�̌���
	
public:
	//tnl::Vector3 line_pos = { 0,0,0 };	//�}�b�v�`�b�v�Ƃ̓����蔻��Ŏg�����W
	//tnl::Vector3 line_prev_pos = { 0,0,0 };	//�}�b�v�`�b�v�Ƃ̓����蔻��Ŏg���ړ��O���W
	//int line_height = 0;	//�}�b�v�`�b�v�Ƃ̓����蔻��Ŏg���L�����N�^�[�̍���

	int attack_effect_graph[10];	//�U���G�t�F�N�g�̉摜�n���h��
	bool attack_flag = false;
	int attack_anim_frame = 0;
	float attack_anim_time = 0;
	int effect_size_width = 25;
	int effect_size_height = 25;
	tnl::Vector3 effect_pos = { 0,0,0 };	//�U���G�t�F�N�g�̍��W

	//int hitcheck_sensor = 0;	//���̃I�u�W�F�N�g�Ɠ��������ꏊ������
	int jumpcount_ = 0;	//�W�����v��
	bool invincible_flag = false;	//���G��Ԃ̃t���O
	bool damage_flag = false;	//�_���[�W�󂯂����̃t���O
	bool life_Decrement_flag = false;

	Player();
	//~Player();
	void life_draw();
	void life_Decrement();
	void get_life(int player_life);
	void updeat(float delta_time);	//�ړ��Ȃ�
	void draw(float delta_time, const Camera& camera);	//�\��
	void jump();	//�W�����v
	void move(float delta_time);	//���E�ړ�
	void damage();	//�_���[�W�A�U�����󂯂���
	void invincible(float delta_time);	//���G
	void attack(float delta_time, const Camera& camera);	//�U��

	float get_player_jumpPower_();
}; 


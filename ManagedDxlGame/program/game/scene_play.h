#pragma once
#include "scene_base.h"
#include "player.h"
#include "enemy.h"
#include "map.h"
#include "hitcheck.h"
#include "background.h"
#include "camera.h"
#include "goal.h"

class Scene_Play : public Scene_Base {
public:
	Scene_Play();
	~Scene_Play();
	//scene_base��update�̉��z�֐��̃I�[�o�[���C�h
	void update(float delta_time) override;
	//scene_base��draw�̉��z�֐��̃I�[�o�[���C�h
	void draw(float delta_time) override;

	//�����蔻��
	void hitchack();

	BackGround* background_ = nullptr;
	Map* map_ = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Camera* camera_ = nullptr;
	Goal* goal_ = nullptr;

private:
	int player_life;
	bool gameover_flag = false;

	std::list<Enemy*> enemys;
	std::list<Block*> blocks;

	int player_enemy_hitcheck = 0;	//�v���C���[�ƃG�l�~�[�̓��������ꏊ�ʂ̎���
	int player_block_hitcheck = 0;	//�v���C���[�ƃu���b�N�̓��������ꏊ�ʂ̎���

	bool seqIdle(float delta_time);

	//sequence_(�ϐ�)��seqIdle(�֐�)�̃A�h���X�����Ă�
	tnl::Sequence<Scene_Play> sequence_ = tnl::Sequence<Scene_Play>(this, &Scene_Play::seqIdle);
};

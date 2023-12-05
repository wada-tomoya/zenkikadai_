#pragma once
#include "../library/tnl_sequence.h"

class Scene_Base;
class Player;
class Bullet;

class GameManager {
public:
	//GameManager* �^��GetInstance�֐����`
	static GameManager* GetInstance(Scene_Base* start_scene = nullptr);

	//GetInstance�̊J���p�֐�
	static void Destroy();

	//�V�[���؂�ւ��p�֐�
	void changescene(Scene_Base* next_scene);

	//���݂̃V�[���ɃA�b�v�f�[�g������֐�
	void scene_update(float delta_time);

private:
	//GameManager�N���X�̃R���X�g���N�^
	GameManager(Scene_Base* start_scene);

	//�V�[�����؂�ւ��̂ɂ����鎞��
	//float trans_time_ = 0.5f;

	//���݂̃V�[���̃A�h���X
	Scene_Base* now_scene_ = nullptr;

	//���̃V�[���̃A�h���X
	Scene_Base* next_scene_ = nullptr;

	tnl::Sequence<GameManager> sequence_ = tnl::Sequence<GameManager>(this, &GameManager::seqRunScene);
	bool seqChangescene(const float delta_time);

	//���X�ɖ��邭����
	//bool seqTransIn(const float delta_time);
	//���X�ɈÂ�����
	//bool seqTransOut(const float delta_time);
	//�������Ȃ����
	bool seqRunScene(const float delta_time);

};
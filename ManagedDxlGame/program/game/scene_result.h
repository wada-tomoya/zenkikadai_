#pragma once
#include "scene_base.h"

class Scene_Rrsult : public Scene_Base {
public:
	//scene_base��update�̉��z�֐��̃I�[�o�[���C�h
	void update(float delta_time) override;
	//scene_base��draw�̉��z�֐��̃I�[�o�[���C�h
	void draw(float delta_time) override;

private:
	bool seqIdle(float delta_time);

	//sequence_(�ϐ�)��seqIdle(�֐�)�̃A�h���X�����Ă�
	tnl::Sequence<Scene_Rrsult> sequence_ = tnl::Sequence<Scene_Rrsult>(this, &Scene_Rrsult::seqIdle);
};
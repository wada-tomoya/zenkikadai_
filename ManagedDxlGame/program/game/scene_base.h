#pragma once

class Scene_Base {
public:
	//���z�֐��Œ�`�@�V�[���̐؂�ւ�
	virtual void update(float delta_time) {};
	//�������z�֐��Œ�`�@�V�[���̕����\��
	virtual void draw(float delta_time) = 0;
};

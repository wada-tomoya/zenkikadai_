#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../library/tnl_sequence.h"

class Character_Base {
public:
	//�̗�
	int hp_ = 0;
	//�ړ����x
	float speed_ = 0;
	//�摜
	int graph_ = 0;	
	//�摜�A�����蔻��T�C�Y
	int size_ = 0;	
	int size_width = 0;
	int size_height = 0;
	//���W
	tnl::Vector3 pos_ = { 0,0,0, };
	//�W�����v�p�d��
	float gravity_ = 0;	
	//�ړ��O���W
	tnl::Vector3 prev_pos;	
};
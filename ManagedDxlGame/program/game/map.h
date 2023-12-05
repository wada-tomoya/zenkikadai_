#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "camera.h"

class Map {
public:
	Map();
	~Map();

	//�}�b�v�`�b�v�̏c���T�C�Y
	const int mapchip_size = 32;

	//�}�b�v�`�b�v�̉摜������ϐ�
	int mapchip_graph[2] ;

	//csv�t�@�C���̃f�[�^��������ϐ�
	std::vector<std::vector<int>> csv_int_data;

	//�}�b�v�`�b�v�\��
	void draw(const Camera& camera);
};
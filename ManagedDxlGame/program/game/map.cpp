#include "map.h"

Map::Map() {
	//�}�b�v�`�b�v�摜�ǂݍ���
	mapchip_graph[1] = LoadGraph("graphics/mapchip/mapchip.png");	//�n�ʃ`�b�v
	//mapchip_graph[0] = LoadGraph("graphics/mapchip/mapchip_2.png");	//

	//csv�t�@�C���ǂݍ���
	csv_int_data = tnl::LoadCsv<int>("csv/map.csv");
}

Map::~Map() {
	mapchip_graph;

	csv_int_data;
}

void Map::draw(const Camera& camera) {

	//�}�b�v�`�b�v�\��
	for (int y = 0; y < csv_int_data.size(); ++y) {
		for (int x = 0; x < csv_int_data[y].size(); ++x) {
			tnl::Vector3 draw_pos = tnl::Vector3(x * mapchip_size, y * mapchip_size, 0) 
				- camera.target_ + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
			DrawGraph( draw_pos.x, draw_pos.y, mapchip_graph[ csv_int_data[y][x] ], true);
		}
	}
}
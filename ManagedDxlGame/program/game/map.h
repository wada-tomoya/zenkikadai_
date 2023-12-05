#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "camera.h"

class Map {
public:
	Map();
	~Map();

	//マップチップの縦横サイズ
	const int mapchip_size = 32;

	//マップチップの画像が入る変数
	int mapchip_graph[2] ;

	//csvファイルのデータをを入れる変数
	std::vector<std::vector<int>> csv_int_data;

	//マップチップ表示
	void draw(const Camera& camera);
};
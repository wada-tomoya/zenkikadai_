#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "../library/tnl_sequence.h"

class Character_Base {
public:
	//体力
	int hp_ = 0;
	//移動速度
	float speed_ = 0;
	//画像
	int graph_ = 0;	
	//画像、当たり判定サイズ
	int size_ = 0;	
	int size_width = 0;
	int size_height = 0;
	//座標
	tnl::Vector3 pos_ = { 0,0,0, };
	//ジャンプ用重力
	float gravity_ = 0;	
	//移動前座標
	tnl::Vector3 prev_pos;	
};
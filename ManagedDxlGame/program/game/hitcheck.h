#pragma once
#include "../library/tnl_sequence.h"
#include "../dxlib_ext/dxlib_ext.h"

//マップチップにつける当たり判定のブロック
class Block {
public:
	Block(const tnl::Vector3& pos) : pos_(pos) {}

	tnl::Vector3 pos_ = { 0,0,0 };
};
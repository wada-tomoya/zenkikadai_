#pragma once
#include "../library/tnl_sequence.h"
#include "../dxlib_ext/dxlib_ext.h"

//�}�b�v�`�b�v�ɂ��铖���蔻��̃u���b�N
class Block {
public:
	Block(const tnl::Vector3& pos) : pos_(pos) {}

	tnl::Vector3 pos_ = { 0,0,0 };
};
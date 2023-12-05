#include "../dxlib_ext/dxlib_ext.h"
#include "bullet.h"

void Bullet::updeat() {
	pos_.x = 10;
}

void Bullet::draw() {
	DrawCircle(pos_.x, pos_.y, rad_, -1, false);
}
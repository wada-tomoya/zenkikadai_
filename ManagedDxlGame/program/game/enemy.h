#pragma once
#include "character.h"
#include "camera.h"

class Enemy : public Character_Base {
private:
	float movetime = 3;	//１行動の時間

	float gravity_max = 10.0f;
	float jumpPower_ = -15.0f;
public:

	float gravity_ = 0;

	bool death_flag = false;

	Enemy(tnl::Vector3 pos);
	void updeat(float delta_time);	//移動など
	void draw(const Camera& camera);	//表示

	void death(float delta_time);

	TNL_CO_SEQUENCE(Enemy, &Enemy::move1)
	bool move1(const float delta_time);
	bool move2(const float delta_time);
	bool move3(const float delta_time);
	bool move4(const float delta_time);
	//bool jump(const float delta_time);
};

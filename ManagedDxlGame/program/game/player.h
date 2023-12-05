#pragma once
#include "character.h"
#include "camera.h"



enum {
	DIR_RIGHT,
	DIR_LEFT,
	DIR_MAX
};

enum direction {
	Right_,
	Left_
};

//struct ATTACK {
//	int attack_effect_graph[10];	//攻撃エフェクトの画像ハンドル
//	bool attack_flag = false;
//	int attack_anim_frame = 0;
//	float attack_anim_time = 0;
//	const int effect_size_width = 30;
//	const int effect_size_height = 30;
//	tnl::Vector3 effect_pos = { 0,0,0 };	//攻撃エフェクトの座標
//};

class Player : public Character_Base {
private:
	int life_ = 3;
	int life_graph = 0;
	int life_graphsize = 32;
	tnl::Vector3 life_pos = { 50,100,0 };

	float jumpPower_ = -15.0f;
	int jumpmax_ = 1;	//ジャンプ回数max
	float gravity_max = 10.0f;
	float enemy_stepon_jumpPower_ = -10.0f;

	bool turnflag = false;	//左右の振り向き、trueは左falseは右

	bool flashing = true;	//キャラクターの点滅
	float flashing_count = 0;	//点滅のカウント

	float invincible_count = 0;	//無敵時間のカウント
	float invincible_time = 2;	//無敵時間

	float anim_time = 0;
	int anim_dir = DIR_RIGHT;	//再生するアニメーション
	int anim_frame = 0;			//アニメーションのフレーム
	int anim_hdls[DIR_MAX][4];

	int direction_ = Right_;	//キャラクターの向き
	
public:
	//tnl::Vector3 line_pos = { 0,0,0 };	//マップチップとの当たり判定で使う座標
	//tnl::Vector3 line_prev_pos = { 0,0,0 };	//マップチップとの当たり判定で使う移動前座標
	//int line_height = 0;	//マップチップとの当たり判定で使うキャラクターの高さ

	int attack_effect_graph[10];	//攻撃エフェクトの画像ハンドル
	bool attack_flag = false;
	int attack_anim_frame = 0;
	float attack_anim_time = 0;
	int effect_size_width = 25;
	int effect_size_height = 25;
	tnl::Vector3 effect_pos = { 0,0,0 };	//攻撃エフェクトの座標

	//int hitcheck_sensor = 0;	//他のオブジェクトと当たった場所を識別
	int jumpcount_ = 0;	//ジャンプ回数
	bool invincible_flag = false;	//無敵状態のフラグ
	bool damage_flag = false;	//ダメージ受けた時のフラグ
	bool life_Decrement_flag = false;

	Player();
	//~Player();
	void life_draw();
	void life_Decrement();
	void get_life(int player_life);
	void updeat(float delta_time);	//移動など
	void draw(float delta_time, const Camera& camera);	//表示
	void jump();	//ジャンプ
	void move(float delta_time);	//左右移動
	void damage();	//ダメージ、攻撃を受けた時
	void invincible(float delta_time);	//無敵
	void attack(float delta_time, const Camera& camera);	//攻撃

	float get_player_jumpPower_();
}; 


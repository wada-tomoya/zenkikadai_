#pragma once
#include "../library/tnl_sequence.h"

class Scene_Base;
class Player;
class Bullet;

class GameManager {
public:
	//GameManager* 型のGetInstance関数を定義
	static GameManager* GetInstance(Scene_Base* start_scene = nullptr);

	//GetInstanceの開放用関数
	static void Destroy();

	//シーン切り替え用関数
	void changescene(Scene_Base* next_scene);

	//現在のシーンにアップデートをする関数
	void scene_update(float delta_time);

private:
	//GameManagerクラスのコンストラクタ
	GameManager(Scene_Base* start_scene);

	//シーンが切り替わるのにかかる時間
	//float trans_time_ = 0.5f;

	//現在のシーンのアドレス
	Scene_Base* now_scene_ = nullptr;

	//次のシーンのアドレス
	Scene_Base* next_scene_ = nullptr;

	tnl::Sequence<GameManager> sequence_ = tnl::Sequence<GameManager>(this, &GameManager::seqRunScene);
	bool seqChangescene(const float delta_time);

	//徐々に明るくする
	//bool seqTransIn(const float delta_time);
	//徐々に暗くする
	//bool seqTransOut(const float delta_time);
	//何もしない状態
	bool seqRunScene(const float delta_time);

};
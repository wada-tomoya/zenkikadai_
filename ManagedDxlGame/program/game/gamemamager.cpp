#include "../dxlib_ext/dxlib_ext.h"
#include "gamemanager.h"
#include "scene_base.h"

//コンストラクタ　									↓now_scene_ = start_scene;
GameManager::GameManager(Scene_Base* start_scene) : now_scene_(start_scene) {}

GameManager* GameManager::GetInstance(Scene_Base* start_scene) {
	//instanceにGameManagerのアドレスを返す
	static GameManager* instance = nullptr;
	if (!instance) {
		instance = new GameManager(start_scene);
	}
	return instance;
}

void GameManager::Destroy() {
	//GetInstanceの開放（アドレスの開放）
	delete GetInstance();
}

void GameManager::changescene(Scene_Base* next_scene) {
	//シーンの切り替え
	next_scene_ = next_scene;

	sequence_.change(&GameManager::seqChangescene);
}

void GameManager::scene_update(float delta_time) {
	// シーンのアップデートの描画
	if (now_scene_) now_scene_->update(delta_time);
	if (now_scene_) now_scene_->draw(delta_time);

	sequence_.update(delta_time);
}

bool GameManager::seqChangescene(const float delta_time) {
	sequence_.change(&GameManager::seqRunScene);

	delete now_scene_;
	now_scene_ = nullptr;
	now_scene_ = next_scene_;
	return true;
}

bool GameManager::seqRunScene(const float delta_time) {
	return true;
}
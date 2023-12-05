#include "../dxlib_ext/dxlib_ext.h"
#include "scene_title.h"
#include "scene_result.h"
#include "gamemanager.h"

void Scene_Rrsult::update(float delta_time) {
	//sequence_のアップデート(実行)
	sequence_.update(delta_time);
}

void Scene_Rrsult::draw(float delta_time) {
	DrawStringEx(500, 300,-1, "GAME OVER", true);
}

bool Scene_Rrsult::seqIdle(float delta_time) {
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		GameManager* mgr = GameManager::GetInstance();
		mgr->changescene(new (Scene_Title));
	}
	return true;
}
#include "../dxlib_ext/dxlib_ext.h"
#include "scene_title.h"
#include "scene_play.h"
#include "gamemanager.h"

//Scene_Title::Scene_Title() {
//	sound_gbm = LoadSoundMem("graphics/sound/bgm_01.mp3");
//}

void Scene_Title::update(float delta_time) {
	//sequence_のアップデート(実行)
	sequence_.update(delta_time);

	//PlaySoundMem(sound_gbm, DX_PLAYTYPE_LOOP, true);
}

void Scene_Title::draw(float delta_tie) {
	SetFontSize(50);
	DrawStringEx(500, 300, -1, "Please Enter", true);
}

bool Scene_Title::seqIdle(float delta_time) {
	//エンターを押したらシーン切り替え
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		GameManager* mgr = GameManager::GetInstance();
		mgr->changescene(new Scene_Play());
	}
	return true;
}

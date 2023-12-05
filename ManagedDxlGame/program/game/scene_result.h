#pragma once
#include "scene_base.h"

class Scene_Rrsult : public Scene_Base {
public:
	//scene_baseのupdateの仮想関数のオーバーライド
	void update(float delta_time) override;
	//scene_baseのdrawの仮想関数のオーバーライド
	void draw(float delta_time) override;

private:
	bool seqIdle(float delta_time);

	//sequence_(変数)にseqIdle(関数)のアドレスを入れてる
	tnl::Sequence<Scene_Rrsult> sequence_ = tnl::Sequence<Scene_Rrsult>(this, &Scene_Rrsult::seqIdle);
};
#pragma once

class Scene_Base {
public:
	//仮想関数で定義　シーンの切り替え
	virtual void update(float delta_time) {};
	//純粋仮想関数で定義　シーンの文字表示
	virtual void draw(float delta_time) = 0;
};

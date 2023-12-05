#pragma once
#include "scene_base.h"
#include "player.h"
#include "enemy.h"
#include "map.h"
#include "hitcheck.h"
#include "background.h"
#include "camera.h"
#include "goal.h"

class Scene_Play : public Scene_Base {
public:
	Scene_Play();
	~Scene_Play();
	//scene_baseのupdateの仮想関数のオーバーライド
	void update(float delta_time) override;
	//scene_baseのdrawの仮想関数のオーバーライド
	void draw(float delta_time) override;

	//当たり判定
	void hitchack();

	BackGround* background_ = nullptr;
	Map* map_ = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Camera* camera_ = nullptr;
	Goal* goal_ = nullptr;

private:
	int player_life;
	bool gameover_flag = false;

	std::list<Enemy*> enemys;
	std::list<Block*> blocks;

	int player_enemy_hitcheck = 0;	//プレイヤーとエネミーの当たった場所別の識別
	int player_block_hitcheck = 0;	//プレイヤーとブロックの当たった場所別の識別

	bool seqIdle(float delta_time);

	//sequence_(変数)にseqIdle(関数)のアドレスを入れてる
	tnl::Sequence<Scene_Play> sequence_ = tnl::Sequence<Scene_Play>(this, &Scene_Play::seqIdle);
};

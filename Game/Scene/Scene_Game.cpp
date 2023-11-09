#include "Scene_Game.h"

Scene_Game::Scene_Game() { Init(); }

Scene_Game::~Scene_Game() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void Scene_Game::Init() {

	game_ = new Game();
	isBack_ = false;
	backCT_ = 0;

}


/*================================================================
	更新処理関数
================================================================*/
void Scene_Game::Update() {

	// 次のシーンへの条件(仮)
	if (Inputs::IsTriggerKey(DIK_RETURN)) {
		nextSceneNo_ = SCENE::TITLE;
	}

	// このシーンの初期化条件
	if (preSceneNo_ != SCENE::GAME) {
		Init();
	}

	// ----- Back ----- //
	if (isBack_) {

		if (backCT_ > 0) {
			
			backCT_--;
		} else {

			isBack_ = false;
		}

	} else {

		if (!sGame_.empty()) {
			if (Inputs::IsTriggerKey(DIK_R)) {

				isBack_ = true;
				backCT_ = 90;

				game_->SetGame(sGame_.top());
				//*game_ = sGame_.top();
				sGame_.pop();
			}
		}

	}


	// ----- Update ----- //
	game_->Update();


	// ----- stack ----- //
	if (game_->GetCowherd()->GetIsStack()) {
		sGame_.push(*game_);
	}



	// ゲームクリア
	if (game_->GetIsGameClear()) {
		nextSceneNo_ = SCENE::GAMECLEAR;
	}

}


/*================================================================
	描画処理関数
================================================================*/
void Scene_Game::Draw() {

	game_->Draw();

}


/*================================================================
	終了処理関数
================================================================*/
void Scene_Game::Finalize() {

	
}

#include <Novice.h>
#include <SafeDelete.h>
#include "Inputs.h"
#include "SceneManager.h"
#include "Environment.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowSize.x, kWindowSize.y);

	// キー入力結果を受け取る箱
	Inputs::SetInstance();

	// シーンマネージャー
	SceneManager* sceneManager = new SceneManager();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		Inputs::Update();

		// シーンの更新 & 描画処理
		sceneManager->Run();

		
		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (Inputs::IsTriggerKey(DIK_ESCAPE)) {
			break;
		}
	}

	// メモリの解放
	SafeDelete(sceneManager);


	// ライブラリの終了
	Novice::Finalize();
		return 0;
}

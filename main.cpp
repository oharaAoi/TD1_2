#include <Novice.h>
#include <SafeDelete.h>
#include "InputManager.h"
#include "SceneManager.h"

const char kWindowTitle[] = "LC1A_06_オオハラアオイ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	InputManager* input = InputManager::GetInstance();

	// シーンマネージャー
	SceneManager* sceneManager = new SceneManager();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		input->Update();

		// シーンの更新 & 描画処理
		sceneManager->Run();

		
		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (input->IsTriggerKey(DIK_ESCAPE)) {
			break;
		}
	}

	// メモリの解放
	SafeDelete(sceneManager);


	// ライブラリの終了
	Novice::Finalize();
		return 0;
}

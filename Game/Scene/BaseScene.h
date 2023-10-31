#pragma once

/// <summary>
/// シーンNoの種類
/// </summary>
enum SCENE {
	TITLE,
	GAME
};


class BaseScene {
protected: // 継承先でも利用

	// 今のシーン
	static int nowSceneNo_;
	// 前のシーン
	static int preSceneNo_;
	// 次のシーン
	static int nextSceneNo_;

public:
	// 仮想デストラクタ
	virtual ~BaseScene();

	// 純粋仮想関数; 継承先で定義を行う
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;

	// シーンNoの更新処理
	static void SceneNoUpdate();

	static int GetNowSceneNo(void) { return nowSceneNo_; }
	static int GetPreSceneNo(void) { return preSceneNo_; }
	static int GetNextSceneNo(void) { return nextSceneNo_; }

};


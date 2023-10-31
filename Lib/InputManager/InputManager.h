#pragma once
#include <Novice.h>
#include "MyVector2.h"

class InputManager final {
private:

	// constructor & destructor
	InputManager(void);
	~InputManager() = default;

	// キーの入力状況
	char keys_[256] = { 0 };
	char preKeys_[256] = { 0 };

	// マウスのリリース入力に使う
	bool preMouseBottom_[7] = { 0 };
	// マウスの位置
	Vec2 mousePos_;

	// コントローラーのリリース入力に使用
	//bool preButton

public:

	// copyConstructor 禁止
	InputManager(const InputManager& obj) = delete;
	// 代入演算子の禁止
	InputManager& operator=(const InputManager& obj) = delete;

	// インスタンス確保用関数
	static InputManager* GetInstance();

	// 更新
	void Update(void);

	// キーボードの入力
	bool IsTriggerKey(int DIK) const;
	bool IsPressKey(int DIK) const;
	bool IsReleaseKey(int DIK) const;

	// マウスの入力; 位置
	bool IsTriggerMouse(int buttonNo) const;
	bool IsPressMouse(int buttonNo) const;
	bool IsReleaseMouse(int buttonNo) const;
	Vec2 GetMousePos(void) { return mousePos_; }
	// ホイールの回転
	int GetWheel(void) const;
	bool IsWheelUP(void) const;
	bool IsWheelDown(void) const;

	// コントローラーの入力;
	bool IsTriggerButton(int stickNo, PadButton button) const;
	bool IsPressButton(int stickNo, PadButton button) const;
	//bool isReleaseButton(int stickNo, PadButton button) const;

};

#include "InputManager.h"
#include <Novice.h>


/// <summary>
/// コンストラクタ
/// </summary>
InputManager::InputManager(void) {}


/// <summary>
/// インスタンス確保
/// </summary>
/// <returns></returns>
InputManager* InputManager::GetInstance() {
	static InputManager instance;
	return &instance;
}


/// <summary>
/// マイフレームの処理
/// </summary>
void InputManager::Update(void) {
	// キーボード入力
	memcpy(preKeys_, keys_, 256);
	Novice::GetHitKeyStateAll(keys_);

	// マウス入力
	for (int mi = 0; mi < 7; mi++) {
		preMouseBottom_[mi] = IsPressMouse(mi);
	}

	// マウスの位置を所得
	Novice::GetMousePosition(&mousePos_.x, &mousePos_.y);

}


/// <summary>
/// キーがトリガーされたか
/// </summary>
/// <param name="DIK">keyNumber</param>
/// <returns></returns>
bool InputManager::IsTriggerKey(int DIK) const {
	return keys_[DIK] && !preKeys_[DIK];
}


/// <summary>
/// キーが押されているか
/// </summary>
/// <param name="DIK">keyNumber</param>
/// <returns></returns>
bool InputManager::IsPressKey(int DIK) const {
	return keys_[DIK];
}


/// <summary>
/// キーが離されたか
/// </summary>
/// <param name="DIK">keyNumber</param>
/// <returns></returns>
bool InputManager::IsReleaseKey(int DIK) const {
	return !keys_[DIK] && preKeys_[DIK];
}


/// <summary>
/// 特定マウスボタンがトリガーされたかを得る
/// </summary>
/// <param name="buttonNo">マウスボタンの番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
/// <returns>true : トリガーされた false : トリガーされてない</returns>
bool InputManager::IsTriggerMouse(int buttonNo) const {
	return Novice::IsTriggerMouse(buttonNo);
}


/// <summary>
/// 特定マウスボタンがプレスされたかを得る
/// </summary>
/// <param name="buttonNo">マウスボタンの番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
/// <returns>true : プレスされた false : プレスされてない</returns>
bool InputManager::IsPressMouse(int buttonNo) const {
	return Novice::IsPressMouse(buttonNo);
}


/// <summary>
/// 特定マウスボタンがリリースされたかを得る
/// </summary>
/// <param name="buttonNo">マウスボタンの番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
/// <returns>true : リリースされた false : リリースされてない</returns>
bool InputManager::IsReleaseMouse(int buttonNo) const {
	return !IsPressMouse(buttonNo) && preMouseBottom_[buttonNo];
}

int InputManager::GetWheel(void) const {
	return Novice::GetWheel();
}

bool InputManager::IsWheelUP(void) const {
	return GetWheel() > 0;
}

bool InputManager::IsWheelDown(void) const {
	return GetWheel() < 0;
}


/// <summary>
/// 特定のコントローラーボタンがトリガーされたかを得る
/// </summary>
/// <param name="stickNo">ジョイスティック番号</param>
/// <param name="buttonジョイスティックのボタン番号param>
/// <returns>true : トリガーされた false : トリガーされてない</returns>
bool InputManager::IsTriggerButton(int stickNo, PadButton button) const {
	return Novice::IsTriggerButton(stickNo, button);
}


/// <summary>
/// 特定のコントローラーボタンがプレスされたかを得る
/// </summary>
/// <param name="stickNo">ジョイスティック番号</param>
/// <param name="buttonジョイスティックのボタン番号param>
/// <returns>true : プレスされた false : プレスされてない</returns>
bool InputManager::IsPressButton(int stickNo, PadButton button) const {
	return Novice::IsPressButton(stickNo, button);
}


/// <summary>
/// 特定のコントローラーボタンがリリースされたかを得る
/// </summary>
/// <param name="stickNo">ジョイスティック番号</param>
/// <param name="buttonジョイスティックのボタン番号param>
/// <returns>true : リリースされた false : リリースされてない</returns>
//bool InputManager::isReleaseButton(int stickNo, PadButton button) const {
//	return false;
//}

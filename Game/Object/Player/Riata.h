#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Quad.h"
#include "Matrix3x3.h"
#include "Draw.h"
#include "InputManager.h"
#include "MyMath.h"

#include <Novice.h>

#include <BaseMap.h>

/// <summary>
/// 投げ縄
/// </summary>
class Riata
	: public BaseMap {
private: // member object

	InputManager* input = InputManager::GetInstance();

	Vec2f worldPos_;
	Vec2 worldAddress_;
	Vec2f size_;
	int gh_;
	unsigned int color_;

	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	bool isIdle_;
	bool isStart_;
	bool isCatch_;

	Vec2f moveDir_;

	// 移動元と移動先
	Vec2f startingPos_;
	Vec2f destinationPos_;

	int movingTime_;
	float easeT_;

public: // member method
	// Constructor & Destructor
	Riata();
	~Riata();

	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

	// user method
	void MakeWorldMatrix();

	void MakeScreenMatrix(
		const Matrix3x3& viewMatrix ,
		const Matrix3x3& orthoMatrix ,
		const Matrix3x3& viewportMatrix
	);

	void AddressUpdate();

	// ----- accessor ----- //

	void SetWorldPos(const Vec2f& pos) { worldPos_ = pos; }
	Vec2f GetWorldPos(void) const { return worldPos_; }

	Vec2 GetWorldAddress(void) const { return worldAddress_; }

	void SetIsIdle(bool isIdle) { isIdle_ = isIdle; }
	bool GetIsIdle(void) const { return isIdle_; }

	void SetIsStart(bool isStart) { isStart_ = isStart; }
	bool GetIsStart(void) const { return isStart_; }

	void SetIsCatch(bool isCatch) { isCatch_ = isCatch; }
	bool GetIsCatch(void) const { return isCatch_; }

	void SetMoveDir(const Vec2f& moveDir) { moveDir_ = moveDir; }
	Vec2f GetMoveDir(void) const { return moveDir_; }

	void SetStartingPos(const Vec2f& startingPos) { startingPos_ = startingPos; }
	Vec2f GetStartingPos(void) const { return startingPos_; }

	void SetDestinationPos(const Vec2f& destinationPos) { destinationPos_ = destinationPos; }
	Vec2f GetDestinationPos(void) { return destinationPos_; }


};
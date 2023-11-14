#include "Riata.h"

Riata::Riata() { Init(); }

Riata::~Riata() { Finalize(); }


/*================================================
	初期化関数
================================================*/
void Riata::Init() {

	worldPos_ = { 0.0f,0.0f };
	worldAddress_ = { 0,0 };
	size_ = { 64.0f,64.0f };
	gh_ = Novice::LoadTexture("white1x1.png");
	color_ = 0x00dd0080;

	localVertex_ = {
		{ -size_.x * 0.5f, size_.y * 0.5f },
		{ size_.x * 0.5f, size_.y * 0.5f },
		{ -size_.x * 0.5f, -size_.y * 0.5f },
		{ size_.x * 0.5f, -size_.y * 0.5f }
	};

	MakeWorldMatrix();

	screenMatrix_ = worldMatrix_;
	screenVertex_ = worldVertex_;

	isIdle_ = false;
	isStart_ = false;
	isCatch_ = false;

	moveDir_ = { 0.0f,0.0f };

	startingPos_ = { 0.0f,0.0f };
	destinationPos_ = { 0.0f,0.0f };

	movingTime_ = 0;
	easeT_ = 0.0f;

	moveType_ = kMoveType::GO;

}


/*================================================
	更新処理関数
================================================*/
void Riata::Update() {

	if (!isStart_) { return; }
	isIdle_ = false;

	//worldPos_ += moveDir_ * Vec2f{ 4,4 };

	/*if (easeT_ < 1.0f) {
		movingTime_++;
		easeT_ = movingTime_ / 45.0f;
	} else {
		moveType_ = kMoveType::BACK;
	}*/

	switch (moveType_) {
	case Riata::kMoveType::GO:
		if (easeT_ < 1.0f) {
			movingTime_++;
			easeT_ = movingTime_ / 45.0f;
		}

		worldPos_.x = MyMath::Lerp(easeT_, startingPos_.x, destinationPos_.x);
		worldPos_.y = MyMath::Lerp(easeT_, startingPos_.y, destinationPos_.y);

		if (easeT_ >= 1.0f) {
			movingTime_ = 0;
			easeT_ = 0;
			moveType_ = kMoveType::BACK;
		}

		break;
	case Riata::kMoveType::BACK:
		if (easeT_ < 1.0f) {
			movingTime_++;
			easeT_ = movingTime_ / 45.0f;
		}

		worldPos_.x = MyMath::Lerp(easeT_, destinationPos_.x, startingPos_.x + (moveDir_.x * tileSize_.x));
		worldPos_.y = MyMath::Lerp(easeT_, destinationPos_.y, startingPos_.y + (moveDir_.y * tileSize_.y));

		if (easeT_ >= 1.0f) {
			isStart_ = false;
			movingTime_ = 0;
			easeT_ = 0;
			moveDir_ = { 0.0f,0.0f };
			moveType_ = kMoveType::GO;
		}
		break;
	}

	/*if (easeT_ >= 1.0f) {
		isStart_ = false;
		movingTime_ = 0;
		easeT_ = 0;
		moveDir_ = { 0.0f,0.0f };
	}*/

	AddressUpdate();
	MakeWorldMatrix();

}


/*================================================
	描画処理関数
================================================*/
void Riata::Draw() {

	// スクリーン座標系での描画
	Draw::Quad(
		screenVertex_,
		{ 0.0f,0.0f },
		{ 1.0f,1.0f },
		gh_,
		color_
	);

	DebugDraw();

}


/*================================================
	終了処理関数
================================================*/
void Riata::Finalize() {


}



/*================================================
	その他メンバ関数
================================================*/


void Riata::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldPos_);
	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);
}

void Riata::MakeScreenMatrix(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {
	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);
	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);
}

void Riata::AddressUpdate() {
	worldAddress_ =
	{ static_cast<int>(worldPos_.x / tileSize_.x),
		static_cast<int>(worldPos_.y / tileSize_.y) };
}

void Riata::DebugDraw() {

	Novice::ScreenPrintf(0, 700, "isStart = %d", isStart_);
	Novice::ScreenPrintf(0, 680, "isIdle = %d", isIdle_);
	ImGui::Begin("Riata : member object");

	if (ImGui::TreeNode("Flag")) {
		//ImGui::SliderInt("isStart_:", &isStart_, false, true);
	}
	ImGui::End();
}

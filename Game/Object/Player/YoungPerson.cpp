#include "YoungPerson.h"

YoungPerson::YoungPerson() { Init(); }

YoungPerson::~YoungPerson() { Finalize(); }

/*================================================================
	初期化関数
================================================================*/
void YoungPerson::Init() {

	// youngの配列の数
	indexMax = 0;

	// ここでcsvファイルのYANGMANの数を数える
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			if (mapAdd_[row][col] == ChipType::YANGMAN) {
				indexMax++;
			}
		}
	}

	// youngの配列に要素数を入れる(宣言時のyoungは何も入っていないためこれをしないとエラー)
	young_.resize(indexMax);

	// youngを初期化するために使う
	int index = 0;

	// タイルのサイズ
	size_ = tileSize_;

	// ワールド空間での中心点
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			if (mapAdd_[row][col] == ChipType::YANGMAN) {
				young_[index].worldCenterPos = {
					col * tileSize_.x + (size_.x * 0.5f),
					row * tileSize_.y + (size_.y * 0.5f)
				};
				index++;
			}
		}
	}

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -size_.x * 0.5f, size_.y * 0.5f },
		{ size_.x * 0.5f, size_.y * 0.5f },
		{ -size_.x * 0.5f, -size_.y * 0.5f },
		{ size_.x * 0.5f, -size_.y * 0.5f },
	};

	MakeWorldMatrix();

	// ローカル空間以外の各行列
	for (int i = 0; i < indexMax; i++) {
		young_[i].screenMatrix = young_[i].worldMatrix;

		// ワールドとスクリーン空間での各頂点座標
		young_[i].screenVertex = young_[i].worldVertex;

		// 移動できる状態かどうか
		young_[i].isMoveIdle = false;

		// 移動方向
		young_[i].moveDir.x = 0.0f;
		young_[i].moveDir.y = 0.0f;

		// 移動量
		young_[i].moveValue.x = 1.0f;
		young_[i].moveValue.y = 1.0f;

		young_[i].moveDirAdd[top].add = { young_[i].centerAdd.x,  young_[i].centerAdd.y + 1 };
		young_[i].moveDirAdd[bottom].add = { young_[i].centerAdd.x,  young_[i].centerAdd.y - 1 };
		young_[i].moveDirAdd[left].add = { young_[i].centerAdd.y - 1, young_[i].centerAdd.y };
		young_[i].moveDirAdd[right].add = { young_[i].centerAdd.y + 1, young_[i].centerAdd.y };

		for (int j = 0; j < 4; j++) {
			young_[i].moveDirAdd[j].center = {
				young_[i].moveDirAdd[j].add.x * tileSize_.x,
				young_[i].moveDirAdd[j].add.y * tileSize_.y
			};
			young_[i].moveDirAdd[j].worldVertex.lt = Transform(localVertex_.lt, young_[i].worldMatrix);
			young_[i].moveDirAdd[j].worldVertex.rt = Transform(localVertex_.rt, young_[i].worldMatrix);
			young_[i].moveDirAdd[j].worldVertex.lb = Transform(localVertex_.lb, young_[i].worldMatrix);
			young_[i].moveDirAdd[j].worldVertex.rb = Transform(localVertex_.rb, young_[i].worldMatrix);
			young_[i].moveDirAdd[j].screenVertex = young_[i].moveDirAdd[j].worldVertex;

			young_[i].canMoveDir[j] = false;
		}

	}

	//各アドレスの計算
	CenterAddUpDate();
}


/*================================================================
	更新処理関数
================================================================*/
void YoungPerson::Update() {

	Move();
	CenterAddUpDate();

}


/*================================================================
	描画処理関数
================================================================*/
void YoungPerson::Draw() {
	for (int i = 0; i < indexMax; i++) {
		Draw::Quad(young_[i].screenVertex, { 0.0f,64.0f }, { 64.0f,64.0f }, GH_, 0xFFFFFFFF);

		for (int j = 0; j < 4; j++) {

			Draw::Quad(young_[i].moveDirAdd[j].screenVertex, { 0.0f,0.0f }, { 1.0f,1.0f }, GH_, 0xFFFFFFFF);

		}

	}

	DebugDraw();

}


/*================================================================
	終了処理関数
================================================================*/
void YoungPerson::Finalize() {



}


/*================================================================
	その他メンバ関数
================================================================*/

void YoungPerson::Move() {

	for (int i = 0; i < indexMax; i++) {
		if (young_[i].isMoveIdle) {

			// 移動待機状態の解除
			if (input->IsTriggerMouse(1)) {
				young_[i].isMoveIdle = false;
			}

			young_[i].moveDir = { 0.0f,0.0f };
			if (input->IsTriggerMouse(0)) {

				// 上
				if (Collision::Rect::Point(
					{ young_[i].centerAdd.x * tileSize_.x, (young_[i].centerAdd.y + 1) * tileSize_.y },
					size_,
					{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

					young_[i].moveDir = { 0.0f,1.0f };
				}

				// 下
				if (Collision::Rect::Point(
					{ young_[i].centerAdd.x * tileSize_.x, (young_[i].centerAdd.y - 1) * tileSize_.y },
					size_,
					{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

					young_[i].moveDir = { 0.0f,-1.0f };
				}

				// 左
				if (Collision::Rect::Point(
					{ (young_[i].centerAdd.x - 1) * tileSize_.x, young_[i].centerAdd.y * tileSize_.y },
					size_,
					{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

					young_[i].moveDir = { -1.0f,0.0f };
				}

				// 右
				if (Collision::Rect::Point(
					{ (young_[i].centerAdd.x + 1) * tileSize_.x, young_[i].centerAdd.y * tileSize_.y },
					size_,
					{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

					young_[i].moveDir = { 1.0f,0.0f };
				}

			}

			young_[i].worldCenterPos += young_[i].moveDir * young_[i].moveValue;


		} else {

			// スクリーン上でマウスが若人に当たっていたら
			if (Collision::Rect::Point(young_[i].screenVertex,
				{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

				if (input->IsTriggerMouse(0)) {
					// 移動待機状態にする
					young_[i].isMoveIdle = true;

				}
			}
		}
	}

}

void YoungPerson::CenterAddUpDate() {
	for (int i = 0; i < indexMax; i++) {
		young_[i].centerAdd.x = static_cast<int>(young_[i].worldCenterPos.x / tileSize_.x);
		young_[i].centerAdd.y = static_cast<int>(young_[i].worldCenterPos.y / tileSize_.y);
	}
}

void YoungPerson::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {
	for (int i = 0; i < indexMax; i++) {
		young_[i].screenMatrix = MakeWvpVpMatrix(young_[i].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

		young_[i].screenVertex.lt = Transform(localVertex_.lt, young_[i].screenMatrix);
		young_[i].screenVertex.rt = Transform(localVertex_.rt, young_[i].screenMatrix);
		young_[i].screenVertex.lb = Transform(localVertex_.lb, young_[i].screenMatrix);
		young_[i].screenVertex.rb = Transform(localVertex_.rb, young_[i].screenMatrix);

	}
}

void YoungPerson::MakeWorldMatrix() {

	for (int i = 0; i < indexMax; i++) {
		young_[i].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, young_[i].worldCenterPos);

		young_[i].worldVertex.lt = Transform(localVertex_.lt, young_[i].worldMatrix);
		young_[i].worldVertex.rt = Transform(localVertex_.rt, young_[i].worldMatrix);
		young_[i].worldVertex.lb = Transform(localVertex_.lb, young_[i].worldMatrix);
		young_[i].worldVertex.rb = Transform(localVertex_.rb, young_[i].worldMatrix);
	}
}

void YoungPerson::DebugDraw() {
	for (int i = 0; i < indexMax; i++) {

		Novice::ScreenPrintf(640, 20 * i, "isMoveIdle = %d", young_[i].isMoveIdle);
	}
}

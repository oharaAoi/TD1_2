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
	CenterAddUpDate();

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



		for (int j = 0; j < 4; j++) {
			young_[i].moveDirAdd[j].worldCenterPos = {
				young_[i].moveDirAdd[j].add.x * tileSize_.x + (size_.x * 0.5f),
				young_[i].moveDirAdd[j].add.y * tileSize_.y + (size_.y * 0.5f)
			};

			young_[i].moveDirAdd[j].screenVertex = young_[i].moveDirAdd[j].worldVertex;

			young_[i].canMoveDir[j] = false;
		}

		young_[i].isMove = false;

	}

}


/*================================================================
	更新処理関数
================================================================*/
void YoungPerson::Update() {

	Move();

	CenterAddUpDate();
	MakeWorldMatrix();


}


/*================================================================
	描画処理関数
================================================================*/
void YoungPerson::Draw() {
	for (int i = 0; i < indexMax; i++) {
		Draw::Quad(young_[i].screenVertex, { 0.0f,64.0f }, { 64.0f,64.0f }, GH_, 0xFFFFFFFF);

		for (int j = 0; j < 4; j++) {

			// 移動できるマスの描画
			if (young_[i].canMoveDir[j] && young_[i].isMoveIdle) {
				Draw::Quad(young_[i].moveDirAdd[j].screenVertex, { 0.0f,0.0f }, { 1.0f,1.0f }, GH_, 0xFFFFFF50);
			}
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

		// フレーム単位での移動したかのフラグ
		young_[i].isMove = false;
		if (young_[i].isMoveIdle) {

			// 移動待機状態の解除
			if (input->IsTriggerMouse(1)) {
				young_[i].isMoveIdle = false;
			}

			// 移動先の選択
			young_[i].moveDir = { 0.0f,0.0f };
			if (input->IsTriggerMouse(0)) {

				// 上
				if (young_[i].canMoveDir[top]) {
					if (Collision::Rect::Point(
						young_[i].moveDirAdd[top].screenVertex,
						{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

						young_[i].moveDir = { 0.0f,1.0f };
						young_[i].isMove = true;
					}
				}

				// 下
				if (young_[i].canMoveDir[bottom]) {
					if (Collision::Rect::Point(
						young_[i].moveDirAdd[bottom].screenVertex,
						{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

						young_[i].moveDir = { 0.0f,-1.0f };
						young_[i].isMove = true;
					}
				}

				// 左
				if (young_[i].canMoveDir[left]) {
					if (Collision::Rect::Point(
						young_[i].moveDirAdd[left].screenVertex,
						{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

						young_[i].moveDir = { -1.0f,0.0f };
						young_[i].isMove = true;
					}
				}

				// 右
				if (young_[i].canMoveDir[right]) {
					if (Collision::Rect::Point(
						young_[i].moveDirAdd[right].screenVertex,
						{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

						young_[i].moveDir = { 1.0f,0.0f };
						young_[i].isMove = true;
					}
				}

			}

			// マスの移動
			young_[i].worldCenterPos += young_[i].moveDir * (young_[i].moveValue * tileSize_);

			if (young_[i].isMove) {

				// 移動したら一度移動可能フラグをへし折る;
				for (int j = 0; j < 4; j++) {
					young_[i].canMoveDir[j] = false;
				}
				young_[i].isMoveIdle = false;
			}


		} else {

			// スクリーン上でマウスが若人に当たっていたら
			if (Collision::Rect::Point(young_[i].screenVertex,
				{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

				if (input->IsTriggerMouse(0)) {
					
					for (int j = 0; j < indexMax; j++) {
						young_[j].isMoveIdle = false;
					}
					
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
		
		// ----- 移動できるマスのアドレスを所得 ----- //
		young_[i].moveDirAdd[top].add.x = young_[i].centerAdd.x;
		young_[i].moveDirAdd[top].add.y = static_cast<int>(young_[i].centerAdd.y + young_[i].moveValue.y);

		young_[i].moveDirAdd[bottom].add.x = young_[i].centerAdd.x;
		young_[i].moveDirAdd[bottom].add.y = static_cast<int>(young_[i].centerAdd.y - young_[i].moveValue.y);

		young_[i].moveDirAdd[left].add.x = static_cast<int>(young_[i].centerAdd.x - young_[i].moveValue.x);
		young_[i].moveDirAdd[left].add.y = young_[i].centerAdd.y;

		young_[i].moveDirAdd[right].add.x = static_cast<int>(young_[i].centerAdd.x + young_[i].moveValue.x);
		young_[i].moveDirAdd[right].add.y = young_[i].centerAdd.y;


		for (int j = 0; j < 4; j++) {
			young_[i].moveDirAdd[j].worldCenterPos = {
				young_[i].moveDirAdd[j].add.x * tileSize_.x + (size_.x * 0.5f),
				young_[i].moveDirAdd[j].add.y * tileSize_.y + (size_.y * 0.5f)
			};
		}
	}

}

void YoungPerson::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {
	for (int i = 0; i < indexMax; i++) {
		young_[i].screenMatrix = MakeWvpVpMatrix(young_[i].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

		young_[i].screenVertex.lt = Transform(localVertex_.lt, young_[i].screenMatrix);
		young_[i].screenVertex.rt = Transform(localVertex_.rt, young_[i].screenMatrix);
		young_[i].screenVertex.lb = Transform(localVertex_.lb, young_[i].screenMatrix);
		young_[i].screenVertex.rb = Transform(localVertex_.rb, young_[i].screenMatrix);

		for (int j = 0; j < 4; j++) {
			young_[i].moveDirAdd[j].screenMatrix = MakeWvpVpMatrix(young_[i].moveDirAdd[j].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

			young_[i].moveDirAdd[j].screenVertex.lt = Transform(localVertex_.lt, young_[i].moveDirAdd[j].screenMatrix);
			young_[i].moveDirAdd[j].screenVertex.rt = Transform(localVertex_.rt, young_[i].moveDirAdd[j].screenMatrix);
			young_[i].moveDirAdd[j].screenVertex.lb = Transform(localVertex_.lb, young_[i].moveDirAdd[j].screenMatrix);
			young_[i].moveDirAdd[j].screenVertex.rb = Transform(localVertex_.rb, young_[i].moveDirAdd[j].screenMatrix);

		}

	}
}

void YoungPerson::MakeWorldMatrix() {

	for (int i = 0; i < indexMax; i++) {
		young_[i].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, young_[i].worldCenterPos);

		young_[i].worldVertex.lt = Transform(localVertex_.lt, young_[i].worldMatrix);
		young_[i].worldVertex.rt = Transform(localVertex_.rt, young_[i].worldMatrix);
		young_[i].worldVertex.lb = Transform(localVertex_.lb, young_[i].worldMatrix);
		young_[i].worldVertex.rb = Transform(localVertex_.rb, young_[i].worldMatrix);

		for (int j = 0; j < 4; j++) {
			young_[i].moveDirAdd[j].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, young_[i].moveDirAdd[j].worldCenterPos);

			young_[i].moveDirAdd[j].screenVertex.lt = Transform(localVertex_.lt, young_[i].moveDirAdd[j].worldMatrix);
			young_[i].moveDirAdd[j].screenVertex.rt = Transform(localVertex_.rt, young_[i].moveDirAdd[j].worldMatrix);
			young_[i].moveDirAdd[j].screenVertex.lb = Transform(localVertex_.lb, young_[i].moveDirAdd[j].worldMatrix);
			young_[i].moveDirAdd[j].screenVertex.rb = Transform(localVertex_.rb, young_[i].moveDirAdd[j].worldMatrix);

		}

	}
}

void YoungPerson::DebugDraw() {
	for (int i = 0; i < indexMax; i++) {

		Novice::ScreenPrintf(640, 20 * i, "isMoveIdle = %d", young_[i].isMoveIdle);
	}
}

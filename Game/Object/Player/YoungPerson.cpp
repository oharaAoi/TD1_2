#include "YoungPerson.h"

YoungPerson::YoungPerson() { Init(); }

YoungPerson::~YoungPerson() { Finalize(); }

/*================================================================
	初期化関数
================================================================*/
void YoungPerson::Init() {

	// youngの配列の数
	maxYoungIndex_ = 0;

	// ここでcsvファイルのYANGMANの数を数える
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			if (mapAdd_[row][col] == ChipType::YANGMAN) {
				maxYoungIndex_++;
			}
		}
	}

	// youngの配列に要素数を入れる(宣言時のyoungは何も入っていないためこれをしないとエラー)
	young_.resize(maxYoungIndex_);

	// 移動マスのcsvを読み込む
	moveGrid_ = LoadFile("./Resources/player/cowherdMoveValue.csv");
	std::reverse(moveGrid_.begin(), moveGrid_.end());

	moveGridMaxIndex_ = 0;
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			switch (moveGrid_[row][col]) {
			case YoungPerson::CanMove:

				moveGridMaxIndex_++;
				break;
			}
		}
	}

	// 移動マスの配列をある分だけに変更
	for (int i = 0; i < maxYoungIndex_; i++) {
		young_[i].canMoveGrid.resize(moveGridMaxIndex_);
	}

	// csv上の若者の中心を探し保存
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			switch (moveGrid_[row][col]) {
			case YoungPerson::Player:

				for (int i = 0; i < moveGridMaxIndex_; i++) {
					young_[i].localCenterAdd = { col, row };
				}

				break;
			}

		}
	}

	

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
	for (int i = 0; i < maxYoungIndex_; i++) {
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
			young_[i].canMoveDir[j] = false;
		}

		young_[i].isMove = false;

	}

	// アドレスに対応した初期化を行う
	index = 0;
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			switch (moveGrid_[row][col]) {
			case YoungPerson::CanMove:

				// 全員同じ移動マスを使う
				for (int i = 0; i < moveGridMaxIndex_; i++) {

					// アドレス
					young_[i].canMoveGrid[index].localAdd = {
						col - young_[i].localCenterAdd.x,
						row - young_[i].localCenterAdd.y
					};

					young_[i].canMoveGrid[index].worldAdd =
						young_[i].canMoveGrid[index].localAdd + young_[i].centerAdd;

					// ワールド座標での中心
					young_[i].canMoveGrid[index].worldCenterPos = {
						young_[i].canMoveGrid[index].worldAdd.x * tileSize_.x + (size_.x * 0.5f),
						young_[i].canMoveGrid[index].worldAdd.y * tileSize_.y + (size_.y * 0.5f)
					};

					// 頂点座標(ローカル
					young_[i].canMoveGrid[index].localVertex = localVertex_;
					young_[i].canMoveGrid[index].screenVertex = young_[i].canMoveGrid[index].localVertex;

					// ワールド空間行列
					young_[i].canMoveGrid[index].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, young_[i].canMoveGrid[index].worldCenterPos);
					young_[i].canMoveGrid[index].screenMatrix = young_[i].canMoveGrid[index].worldMatrix;

					young_[i].canMoveGrid[index].canMove = false;

				}

				index++;

				break;
			}

		}
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
	for (int i = 0; i < maxYoungIndex_; i++) {

		// 若人を描画
		Draw::Quad(
			young_[i].screenVertex,
			{ 0.0f,64.0f },
			{ 64.0f,64.0f },
			GH_,
			0xFFFFFFFF
		);

		for (int j = 0; j < moveGridMaxIndex_; j++) {

			// 移動マスの描画
			Draw::Quad(
				young_[i].canMoveGrid[j].screenVertex,
				{ 0.0f,0.0f },
				{ 1.0f,1.0f},
				gh_,
				0xFFFFFFFF
			);
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

	for (int i = 0; i < maxYoungIndex_; i++) {

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

					for (int j = 0; j < maxYoungIndex_; j++) {
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

	for (int i = 0; i < maxYoungIndex_; i++) {
		young_[i].centerAdd.x = static_cast<int>(young_[i].worldCenterPos.x / tileSize_.x);
		young_[i].centerAdd.y = static_cast<int>(young_[i].worldCenterPos.y / tileSize_.y);

		// ----- 移動できるマスのアドレスを所得 ----- //



	}

}

void YoungPerson::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {
	for (int i = 0; i < maxYoungIndex_; i++) {
		young_[i].screenMatrix = MakeWvpVpMatrix(young_[i].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

		young_[i].screenVertex.lt = Transform(localVertex_.lt, young_[i].screenMatrix);
		young_[i].screenVertex.rt = Transform(localVertex_.rt, young_[i].screenMatrix);
		young_[i].screenVertex.lb = Transform(localVertex_.lb, young_[i].screenMatrix);
		young_[i].screenVertex.rb = Transform(localVertex_.rb, young_[i].screenMatrix);



	}
}

void YoungPerson::MakeWorldMatrix() {

	for (int i = 0; i < maxYoungIndex_; i++) {
		young_[i].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, young_[i].worldCenterPos);

		young_[i].worldVertex.lt = Transform(localVertex_.lt, young_[i].worldMatrix);
		young_[i].worldVertex.rt = Transform(localVertex_.rt, young_[i].worldMatrix);
		young_[i].worldVertex.lb = Transform(localVertex_.lb, young_[i].worldMatrix);
		young_[i].worldVertex.rb = Transform(localVertex_.rb, young_[i].worldMatrix);



	}
}

void YoungPerson::DebugDraw() {
	for (int i = 0; i < maxYoungIndex_; i++) {

		Novice::ScreenPrintf(640, 20 * i, "isMoveIdle = %d", young_[i].isMoveIdle);
	}
}

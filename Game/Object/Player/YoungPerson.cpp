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
	moveGrid_ = LoadFile("./Resources/player/youngPersonMoveValue.csv");
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

				for (int i = 0; i < maxYoungIndex_; i++) {
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
		young_[i].isMove = false;

	}

	// アドレスに対応した初期化を行う
	index = 0;
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			switch (moveGrid_[row][col]) {
			case YoungPerson::CanMove:

				// 全員同じ移動マスを使う
				for (int i = 0; i < maxYoungIndex_; i++) {

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
	for (int yi = 0; yi < maxYoungIndex_; yi++) {

		// 若人を描画
		Draw::Quad(
			young_[yi].screenVertex,
			{ 0.0f,64.0f },
			{ 64.0f,64.0f },
			GH_,
			0xFFFFFFFF
		);

		for (int gi = 0; gi < moveGridMaxIndex_; gi++) {

			if (young_[yi].canMoveGrid[gi].canMove) {
				// 移動マスの描画
				Draw::Quad(
					young_[yi].canMoveGrid[gi].screenVertex,
					{ 0.0f,0.0f },
					{ 1.0f,1.0f },
					gh_,
					0xdd000050
				);
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

	for (int yi = 0; yi < maxYoungIndex_; yi++) {

		// フレーム単位での移動したかのフラグ
		young_[yi].isMove = false;
		if (young_[yi].isMoveIdle) {

			// 移動待機状態の解除
			if (input->IsTriggerMouse(1)) {
				young_[yi].isMoveIdle = false;
			}

			// 移動先の選択
			if (input->IsTriggerMouse(0)) {

				for (int gi = 0; gi < moveGridMaxIndex_; gi++) {

					if (young_[yi].canMoveGrid[gi].canMove) {

						if (Collision::Rect::Point(
							young_[yi].canMoveGrid[gi].screenVertex,
							{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

							// ワールド座標の更新
							young_[yi].worldCenterPos += {
								young_[yi].canMoveGrid[gi].localAdd.x* tileSize_.x,
									young_[yi].canMoveGrid[gi].localAdd.y* tileSize_.y,
							};

							young_[yi].isMove = true;

						}
					}
				}

			}

			// マスの移動
			//young_[yi].worldCenterPos += young_[yi].moveDir * (young_[yi].moveValue * tileSize_);

			if (young_[yi].isMove) {

				young_[yi].isMoveIdle = false;
			}


		} else {

			// スクリーン上でマウスが若人に当たっていたら
			if (Collision::Rect::Point(young_[yi].screenVertex,
				{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

				if (input->IsTriggerMouse(0)) {

					for (int j = 0; j < maxYoungIndex_; j++) {
						young_[j].isMoveIdle = false;
					}

					// 移動待機状態にする
					young_[yi].isMoveIdle = true;

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
		int index = 0;
		for (int row = 0; row < moveGrid_.size(); row++) {
			for (int col = 0; col < moveGrid_[0].size(); col++) {

				// アドレスに対応した初期化
				switch (moveGrid_[row][col]) {
				case YoungPerson::CanMove:

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

					index++;
					break;
				}

			}

		}


	}

}

void YoungPerson::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {
	for (int i = 0; i < maxYoungIndex_; i++) {
		young_[i].screenMatrix =
			MakeWvpVpMatrix(young_[i].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

		young_[i].screenVertex.lt = Transform(localVertex_.lt, young_[i].screenMatrix);
		young_[i].screenVertex.rt = Transform(localVertex_.rt, young_[i].screenMatrix);
		young_[i].screenVertex.lb = Transform(localVertex_.lb, young_[i].screenMatrix);
		young_[i].screenVertex.rb = Transform(localVertex_.rb, young_[i].screenMatrix);

		for (int j = 0; j < moveGridMaxIndex_; j++) {

			young_[i].canMoveGrid[j].screenMatrix =
				MakeWvpVpMatrix(young_[i].canMoveGrid[j].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

			young_[i].canMoveGrid[j].screenVertex.lt = Transform(young_[i].canMoveGrid[j].localVertex.lt, young_[i].canMoveGrid[j].screenMatrix);
			young_[i].canMoveGrid[j].screenVertex.rt = Transform(young_[i].canMoveGrid[j].localVertex.rt, young_[i].canMoveGrid[j].screenMatrix);
			young_[i].canMoveGrid[j].screenVertex.lb = Transform(young_[i].canMoveGrid[j].localVertex.lb, young_[i].canMoveGrid[j].screenMatrix);
			young_[i].canMoveGrid[j].screenVertex.rb = Transform(young_[i].canMoveGrid[j].localVertex.rb, young_[i].canMoveGrid[j].screenMatrix);

		}
	}
}

void YoungPerson::MakeWorldMatrix() {

	for (int i = 0; i < maxYoungIndex_; i++) {
		young_[i].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, young_[i].worldCenterPos);

		young_[i].worldVertex.lt = Transform(localVertex_.lt, young_[i].worldMatrix);
		young_[i].worldVertex.rt = Transform(localVertex_.rt, young_[i].worldMatrix);
		young_[i].worldVertex.lb = Transform(localVertex_.lb, young_[i].worldMatrix);
		young_[i].worldVertex.rb = Transform(localVertex_.rb, young_[i].worldMatrix);

		for (int j = 0; j < moveGridMaxIndex_; j++) {

			young_[i].canMoveGrid[j].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, young_[i].canMoveGrid[j].worldCenterPos);

		}
	}
}

void YoungPerson::DebugDraw() {
	for (int yi = 0; yi < maxYoungIndex_; yi++) {

		Novice::ScreenPrintf(640, 20 * yi, "isMoveIdle = %d", young_[yi].isMoveIdle);
		for (int gi = 0; gi < moveGridMaxIndex_; gi++) {

			Novice::ScreenPrintf(800, 20 * gi, "[%d]worldAdd : x = %d, y = %d",
				0, young_[0].canMoveGrid[gi].worldAdd.x, young_[0].canMoveGrid[gi].worldAdd.y);

		}

	}
}

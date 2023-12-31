﻿#include "MapChip.h"

MapChip::MapChip() {

	//ステージのファイル
	csvFilePath_[0] = "./Resources/stage/stageTest1.csv";
	csvFilePath_[1] = "./Resources/stage/stageTest2.csv";
	csvFilePath_[2] = "./Resources/stage/stage1.csv";
	csvFilePath_[3] = "./Resources/stage/stage2.csv";
	csvFilePath_[4] = "./Resources/stage/stage3.csv";
	csvFilePath_[5] = "./Resources/stage/stage4.csv";
	csvFilePath_[6] = "./Resources/stage/stage5.csv";
	csvFilePath_[7] = "./Resources/stage/stage6.csv";
	csvFilePath_[8] = "./Resources/stage/stage7.csv";
	csvFilePath_[9] = "./Resources/stage/stage8.csv";
	csvFilePath_[10] = "./Resources/stage/stage9.csv";
	csvFilePath_[11] = "./Resources/stage/stage10.csv";
	csvFilePath_[12] = "./Resources/stage/stage11.csv";
	csvFilePath_[13] = "./Resources/stage/stage12.csv";
	csvFilePath_[14] = "./Resources/stage/stage13.csv";
	csvFilePath_[15] = "./Resources/stage/stage14.csv";
	csvFilePath_[16] = "./Resources/stage/stage15.csv";

	mapAdd_ = LoadFile(csvFilePath_[Scene_LevelSelect::GetSelectStage()]);

	row_ = static_cast<int>(mapAdd_.size());
	col_ = static_cast<int>(mapAdd_[0].size());

	//配列の確保
	mapChip_ = new Base * [row_];
	for (int i = 0; i < row_; i++) {
		mapChip_[i] = new Base[col_];
	}

	Init();
	DebugInit();
}

MapChip::~MapChip() {
	// メモリを解放
	for (int i = 0; i < row_; i++) {
		delete[] mapChip_[i];
	}
	delete[] mapChip_;
}

//==============================================================
void MapChip::Init() {

	//マップの画像の番号の振り分け
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mapTile_[i][j].x = j * tileSize_.x;
			mapTile_[i][j].y = i * tileSize_.y;
		}
	}

	GH_ = Novice::LoadTexture("./Resources/images/mapTile/colorMap.png");

	size_.x = 64.0f;
	size_.y = 64.0f;

	//マップチップの初期化
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			//if (mapAdd_[row][col] != NONE) {
			mapChip_[row][col].pos.x = (tileSize_.x * static_cast<float>(col)) + (tileSize_.x / 2.0f);
			mapChip_[row][col].pos.y = (tileSize_.y * static_cast<float>(row)) + (tileSize_.y / 2.0f);

			mapChip_[row][col].scale.x = 1.0f;
			mapChip_[row][col].scale.y = 1.0f;

			mapChip_[row][col].color = 0xFFFFFFFF;

			// playerなどの初期化のための処理
			if (mapAdd_[row][col] == COWHERD) {
				cowheadPos_ = mapChip_[row][col].pos;

			} else if (mapAdd_[row][col] == YANGMAN) {

			} else if (mapAdd_[row][col] == COW) {
				cowPos_ = mapChip_[row][col].pos;
				cowType_ = kCowType::Calf;

			} else if(mapAdd_[row][col] == BULL) {
				cowType_ = kCowType::Bull;

			} else if (mapAdd_[row][col] == BULLFIGHTING) {
				cowType_ = kCowType::Fighting;
			}


			// 柵のhpを実装するためのもの; 適当な値を入れる
			mapChip_[row][col].hp = 2;
			//}
		}
	}

	//各頂点の初期化
	CalcWorldVertex();
	CalcScreenVertex();

	//原点中心vertex
	localVertex_.lt.x = -32.0f;
	localVertex_.lt.y = 32.0f;

	localVertex_.rt.x = 32.0f;
	localVertex_.rt.y = 32.0f;

	localVertex_.lb.x = -32.0f;
	localVertex_.lb.y = -32.0f;

	localVertex_.rb.x = 32.0f;
	localVertex_.rb.y = -32.0f;

}

//==============================================================

void MapChip::Update() {
}


//==============================================================
void MapChip::Draw() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			switch (mapAdd_[row][col]) {
			case ChipType::STAGEOUT: // 場外

				Draw::Quad(
					mapChip_[row][col].screenVertex,
					mapTile_[0][0],
					tileSize_,
					GH_,
					mapChip_[row][col].color
				);

				break;
			case ChipType::FENCE: // フェンス

				Draw::Quad(
					mapChip_[row][col].screenVertex,
					mapTile_[0][1],
					tileSize_,
					GH_,
					mapChip_[row][col].color
				);

				Novice::ScreenPrintf(
					(int)mapChip_[row][col].screenVertex.lt.x,
					(int)mapChip_[row][col].screenVertex.lt.y + 20,
					"hp:%d",
					mapChip_[row][col].hp
				);

				break;

			case ChipType::ROCK: // 岩

				Draw::Quad(
					mapChip_[row][col].screenVertex,
					mapTile_[1][2],
					tileSize_,
					GH_,
					mapChip_[row][col].color
				);

				break;

			}


			// lt -> rt
			Novice::DrawLine(
				static_cast<int>(mapChip_[row][col].screenVertex.lt.x),
				static_cast<int>(mapChip_[row][col].screenVertex.lt.y),
				static_cast<int>(mapChip_[row][col].screenVertex.rt.x),
				static_cast<int>(mapChip_[row][col].screenVertex.rt.y),
				0xddddddff
			);

			// rt -> rb
			Novice::DrawLine(
				static_cast<int>(mapChip_[row][col].screenVertex.rt.x),
				static_cast<int>(mapChip_[row][col].screenVertex.rt.y),
				static_cast<int>(mapChip_[row][col].screenVertex.rb.x),
				static_cast<int>(mapChip_[row][col].screenVertex.rb.y),
				0xddddddff
			);

			// rb -> lb
			Novice::DrawLine(
				static_cast<int>(mapChip_[row][col].screenVertex.rb.x),
				static_cast<int>(mapChip_[row][col].screenVertex.rb.y),
				static_cast<int>(mapChip_[row][col].screenVertex.lb.x),
				static_cast<int>(mapChip_[row][col].screenVertex.lb.y),
				0xddddddff
			);

			// lb -> lt
			Novice::DrawLine(
				static_cast<int>(mapChip_[row][col].screenVertex.lb.x),
				static_cast<int>(mapChip_[row][col].screenVertex.lb.y),
				static_cast<int>(mapChip_[row][col].screenVertex.lt.x),
				static_cast<int>(mapChip_[row][col].screenVertex.lt.y),
				0xddddddff
			);

			Novice::ScreenPrintf(
				(int)mapChip_[row][col].screenVertex.lt.x,
				(int)mapChip_[row][col].screenVertex.lt.y,
				"%d:%d",
				row, col
			);

		}
	}
}


//==============================================================
void MapChip::CalcScreenVertex() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			mapChip_[row][col].screenVertex.lt.x = mapChip_[row][col].pos.x - (tileSize_.x / 2.0f);
			mapChip_[row][col].screenVertex.lt.y = mapChip_[row][col].pos.y + (tileSize_.y / 2.0f);

			mapChip_[row][col].screenVertex.rt.x = mapChip_[row][col].pos.x + (tileSize_.x / 2.0f);
			mapChip_[row][col].screenVertex.rt.y = mapChip_[row][col].pos.y + (tileSize_.y / 2.0f);

			mapChip_[row][col].screenVertex.lb.x = mapChip_[row][col].pos.x - (tileSize_.x / 2.0f);
			mapChip_[row][col].screenVertex.lb.y = mapChip_[row][col].pos.y - (tileSize_.y / 2.0f);

			mapChip_[row][col].screenVertex.rb.x = mapChip_[row][col].pos.x + (tileSize_.x / 2.0f);
			mapChip_[row][col].screenVertex.rb.y = mapChip_[row][col].pos.y - (tileSize_.y / 2.0f);
		}
	}
}

//==============================================================
void MapChip::CalcWorldVertex() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			mapChip_[row][col].worldVertex.lt.x = mapChip_[row][col].pos.x - (tileSize_.x / 2.0f);
			mapChip_[row][col].worldVertex.lt.y = mapChip_[row][col].pos.y + (tileSize_.y / 2.0f);

			mapChip_[row][col].worldVertex.rt.x = mapChip_[row][col].pos.x + (tileSize_.x / 2.0f);
			mapChip_[row][col].worldVertex.rt.y = mapChip_[row][col].pos.y + (tileSize_.y / 2.0f);

			mapChip_[row][col].worldVertex.lb.x = mapChip_[row][col].pos.x - (tileSize_.x / 2.0f);
			mapChip_[row][col].worldVertex.lb.y = mapChip_[row][col].pos.y - (tileSize_.y / 2.0f);

			mapChip_[row][col].worldVertex.rb.x = mapChip_[row][col].pos.x + (tileSize_.x / 2.0f);
			mapChip_[row][col].worldVertex.rb.y = mapChip_[row][col].pos.y - (tileSize_.y / 2.0f);
		}
	}
}

//==============================================================
void MapChip::MatrixChange(const Matrix3x3& view, const Matrix3x3& ortho, const Matrix3x3& viewport) {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			//if (mapAdd_[row][col] != NONE) {
			worldMatrix_ = MakeAffineMatrix(mapChip_[row][col].scale, 0.0f, mapChip_[row][col].pos);

			screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, view, ortho, viewport);

			//少数点のせいかもしれないけど右と左の差が61になる
			mapChip_[row][col].screenVertex.lt = Transform(localVertex_.lt, screenMatrix_);
			mapChip_[row][col].screenVertex.rt = Transform(localVertex_.rt, screenMatrix_);
			mapChip_[row][col].screenVertex.lb = Transform(localVertex_.lb, screenMatrix_);
			mapChip_[row][col].screenVertex.rb = Transform(localVertex_.rb, screenMatrix_);
			//}
		}
	}
}

void MapChip::MakeWorldMatrix() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			//if (mapAdd_[row][col] != NONE) {
			worldMatrix_ = MakeAffineMatrix(mapChip_[row][col].scale, 0.0f, mapChip_[row][col].pos);

			mapChip_[row][col].worldVertex.lt = Transform(localVertex_.lt, screenMatrix_);
			mapChip_[row][col].worldVertex.rt = Transform(localVertex_.rt, screenMatrix_);
			mapChip_[row][col].worldVertex.lb = Transform(localVertex_.lb, screenMatrix_);
			mapChip_[row][col].worldVertex.rb = Transform(localVertex_.rb, screenMatrix_);
			//}
		}
	}
}

//==============================================================
// ここから先はデバックの物

void MapChip::DebugInit() {
	for (int i = 0; i < 10; i++) {
		xAxis_[i].st.x = 0.0f;
		xAxis_[i].st.y = tileSize_.y * i;
		xAxis_[i].end.x = tileSize_.x * 10;
		xAxis_[i].end.y = tileSize_.y * i;

		//61なのはなんかずれているから(理由はMatrixChangeに)
		yAxis_[i].st.x = tileSize_.x * i;
		yAxis_[i].st.y = 0.0f;
		yAxis_[i].end.x = tileSize_.x * i;
		yAxis_[i].end.y = tileSize_.y * 10;
	}
}

void MapChip::DebugDraw() {
	/*for (int i = 0; i < 10; i++) {
		Novice::DrawLine(
			static_cast<int>(xAxis_[i].st.x + mapChip_[9][0].screenVertex.lt.x),
			static_cast<int>(xAxis_[i].st.y + mapChip_[9][0].screenVertex.lt.y),
			static_cast<int>(xAxis_[i].end.x + mapChip_[9][0].screenVertex.lt.x),
			static_cast<int>(xAxis_[i].end.y + mapChip_[9][0].screenVertex.lt.y),
			0xFFFFFFFF
		);

		Novice::DrawLine(
			static_cast<int>(yAxis_[i].st.x + mapChip_[9][0].screenVertex.lt.x),
			static_cast<int>(yAxis_[i].st.y + mapChip_[9][0].screenVertex.lt.y),
			static_cast<int>(yAxis_[i].end.x + mapChip_[9][0].screenVertex.lt.x),
			static_cast<int>(yAxis_[i].end.y + mapChip_[9][0].screenVertex.lt.y),
			0xFFFFFFFF
		);
	}*/
}


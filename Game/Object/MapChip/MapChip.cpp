#include "MapChip.h"

MapChip::MapChip(){

	//ステージのファイル
	csvFilePath_[0] = "./Resources/stage/stageTest1.csv";

	AddressRevers();

	//配列の確保
	mapChip_ = new Base * [row_];
	for (int i = 0; i < row_; i++) {
		mapChip_[i] = new Base[col_];
	}

	Init();
	DebugInit();
}

MapChip::~MapChip(){
	// メモリを解放
	for (int i = 0; i < row_; i++) {
		delete[] mapChip_[i];
	}
	delete[] mapChip_;
}

//==============================================================
void MapChip::Init(){

	size_.x = 64.0f;
	size_.y = 64.0f;

	//マップの画像の番号の振り分け
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mapTile_[i][j].x = j * size_.x;
			mapTile_[i][j].y = i * size_.y;
		}
	}

	GH_ = Novice::LoadTexture("./Resources/images/mapTile/colorMap.png");

	//マップチップの初期化
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			if (mapAdd_[row][col] != NONE) {
				mapChip_[row][col].pos.x = (size_.x * static_cast<float>(col)) + (size_.x / 2.0f);
				mapChip_[row][col].pos.y = (size_.y * static_cast<float>(row)) + (size_.y / 2.0f);

				mapChip_[row][col].scale.x = 1.0f;
				mapChip_[row][col].scale.y = 1.0f;

				mapChip_[row][col].color = 0xFFFFFFFF;

				// playerなどの初期化のための処理
				if (mapAdd_[row][col] == COWHERD) {
					cowheadPos_ = mapChip_[row][col].pos;

				} else if (mapAdd_[row][col] == YANGMAN) {

				} else if (mapAdd_[row][col] == COW) {
					cowPos_ = mapChip_[row][col].pos;
				}
			}
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

void MapChip::Update(){
}


//==============================================================
void MapChip::Draw(){
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			switch (mapAdd_[row][col]) {
			case ChipType::STAGEOUT: // 場外
				
				Draw::Quad(
					mapChip_[row][col].screenVertex,
					mapTile_[0][0], 
					size_, 
					GH_,
					mapChip_[row][col].color
				);

				break;
			case ChipType::FENCE: // フェンス
				
				Draw::Quad(
					mapChip_[row][col].screenVertex,
					mapTile_[0][1],
					size_, 
					GH_, 
					mapChip_[row][col].color
				);

				break;

			}

			/*if (mapAdd_[row][col] == STAGEOUT) {
				Draw::Quad(mapChip_[row][col].screenVertex, mapTile_[0][0], size_, GH_, mapChip_[row][col].color);
			} else if (mapAdd_[row][col] == FENCE) {
				Draw::Quad(mapChip_[row][col].screenVertex, mapTile_[0][1], size_, GH_, mapChip_[row][col].color);
			} else if (mapAdd_[row][col] == COW) {
				Draw::Quad(mapChip_[row][col].screenVertex, mapTile_[1][1], size_, GH_, mapChip_[row][col].color);
			}*/
		}
	}
}


//==============================================================
void MapChip::CalcScreenVertex() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			mapChip_[row][col].screenVertex.lt.x = mapChip_[row][col].pos.x - (size_.x / 2.0f);
			mapChip_[row][col].screenVertex.lt.y = mapChip_[row][col].pos.y + (size_.y / 2.0f);

			mapChip_[row][col].screenVertex.rt.x = mapChip_[row][col].pos.x + (size_.x / 2.0f);
			mapChip_[row][col].screenVertex.rt.y = mapChip_[row][col].pos.y + (size_.y / 2.0f);

			mapChip_[row][col].screenVertex.lb.x = mapChip_[row][col].pos.x - (size_.x / 2.0f);
			mapChip_[row][col].screenVertex.lb.y = mapChip_[row][col].pos.y - (size_.y / 2.0f);

			mapChip_[row][col].screenVertex.rb.x = mapChip_[row][col].pos.x + (size_.x / 2.0f);
			mapChip_[row][col].screenVertex.rb.y = mapChip_[row][col].pos.y - (size_.y / 2.0f);
		}
	}
}

//==============================================================
void MapChip::CalcWorldVertex() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			mapChip_[row][col].worldVertex.lt.x = mapChip_[row][col].pos.x - (size_.x / 2.0f);
			mapChip_[row][col].worldVertex.lt.y = mapChip_[row][col].pos.y + (size_.y / 2.0f);

			mapChip_[row][col].worldVertex.rt.x = mapChip_[row][col].pos.x + (size_.x / 2.0f);
			mapChip_[row][col].worldVertex.rt.y = mapChip_[row][col].pos.y + (size_.y / 2.0f);

			mapChip_[row][col].worldVertex.lb.x = mapChip_[row][col].pos.x - (size_.x / 2.0f);
			mapChip_[row][col].worldVertex.lb.y = mapChip_[row][col].pos.y - (size_.y / 2.0f);

			mapChip_[row][col].worldVertex.rb.x = mapChip_[row][col].pos.x + (size_.x / 2.0f);
			mapChip_[row][col].worldVertex.rb.y = mapChip_[row][col].pos.y - (size_.y / 2.0f);
		}
	}
}

//==============================================================
void MapChip::MatrixChange(const Matrix3x3& view, const Matrix3x3& ortho, const Matrix3x3& viewport) {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			if (mapAdd_[row][col] != NONE) {
				worldMatrix_ = MakeAffineMatrix(mapChip_[row][col].scale, 0.0f, mapChip_[row][col].pos);

				screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, view, ortho, viewport);

				//少数点のせいかもしれないけど右と左の差が61になる
				mapChip_[row][col].screenVertex.lt = Transform(localVertex_.lt, screenMatrix_);
				mapChip_[row][col].screenVertex.rt = Transform(localVertex_.rt, screenMatrix_);
				mapChip_[row][col].screenVertex.lb = Transform(localVertex_.lb, screenMatrix_);
				mapChip_[row][col].screenVertex.rb = Transform(localVertex_.rb, screenMatrix_);
			}
		}
	}
}

void MapChip::MakeWorldMatrix() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			if (mapAdd_[row][col] != NONE) {
				worldMatrix_ = MakeAffineMatrix(mapChip_[row][col].scale, 0.0f, mapChip_[row][col].pos);

				mapChip_[row][col].worldVertex.lt = Transform(localVertex_.lt, screenMatrix_);
				mapChip_[row][col].worldVertex.rt = Transform(localVertex_.rt, screenMatrix_);
				mapChip_[row][col].worldVertex.lb = Transform(localVertex_.lb, screenMatrix_);
				mapChip_[row][col].worldVertex.rb = Transform(localVertex_.rb, screenMatrix_);
			}
		}
	}
}

//==============================================================
// ここから先はデバックの物

void MapChip::DebugInit() {
	for (int i = 0; i < 10; i++) {
		xAxis_[i].st.x = 0.0f;
		xAxis_[i].st.y = size_.y * i;
		xAxis_[i].end.x = size_.x * 10;
		xAxis_[i].end.y = size_.y * i;

		//61なのはなんかずれているから(理由はMatrixChangeに)
		yAxis_[i].st.x = size_.x * i;
		yAxis_[i].st.y = 0.0f;
		yAxis_[i].end.x = size_.x * i;
		yAxis_[i].end.y = size_.y * 10;
	}
}

void MapChip::DebugDraw() {
	for (int i = 0; i < 10; i++) {
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
	}
}


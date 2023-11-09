#pragma once
#include "ImGuiManager.h"

// ----- object ----- //
#include "BaseCow.h"
#include "BaseMap.h"
#include "MapChip.h"

// ----- Maneger ----- //
#include "Turn.h"


class BullFighting:
	public BaseCow,
	public BaseMap,
	public Turn {

private:

	// csvを読み込む物
	std::vector<std::vector<int>>moveGrid_;

public:

	BullFighting();
	~BullFighting();

/*================================================================
	 default method
================================================================*/
	void Init();
	void Update()override;
	void Draw()override;
	void Finalize()override;

/*================================================================
	 user method
================================================================*/

	void CenterAddUpdate();

	void DireInit();

	void MoveTurn();

	void Move();

	// スクリーン行列と各頂点の計算
	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	// ワールド行列と各頂点の計算
	void MakeWorldMatrix();
};


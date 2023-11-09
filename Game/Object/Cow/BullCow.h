#pragma once

// ----- object ----- //
#include "BaseCow.h"
#include "BaseMap.h"
#include "MapChip.h"

// ----- Maneger ----- //
#include "Turn.h"

class BullCow : 
	public BaseCow,
	public BaseMap,
	public Turn {

private:

	bool isDisplay_;

public:

	BullCow();
	~BullCow();


	/* default method */
	void Init();
	void Update()override;
	void Draw()override;
	void Finalize()override;

	/* user method */
	void CenterAddUpdate();


	// スクリーン行列と各頂点の計算
	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	// ワールド行列と各頂点の計算
	void MakeWorldMatrix();

};
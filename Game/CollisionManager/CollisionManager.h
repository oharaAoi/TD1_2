#pragma once

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"

// ----- math ----- //
#include <SafeDelete.h>

/// <summary>
/// 当たり判定と移動できるかの判定を取るクラス
/// </summary>
class CollisionManager final {
private:

	Cowherd* cowherd_;
	YoungPerson* youngPerson_;
	MapChip* mapChip_;

public:
	// Constructor & Destructor
	CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip);
	~CollisionManager();

	// default method
	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip);
	//void Update();
	//void Draw();
	void Finalize();

	// user method
	void CheckCanMove();

private:

	// user method
	void CowherdCanMove();
	bool CowherdCheckCanMove(const Vec2& add);

	void YoungPersonCanMove();
	bool YoungPersonCheckCanMove(const Vec2& add);


	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

};


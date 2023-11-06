#pragma once

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"
#include "Cow.h"
#include "Dog.h"

// ----- math ----- //
#include <SafeDelete.h>
#include "CanMoveDirection.h"

// ----- lib ----- //
#include "MyMath.h"

// ----- collision ----- //
#include "CowCollision.h"
#include "CowherdCollision.h"

/// <summary>
/// 当たり判定と移動できるかの判定を取るクラス
/// </summary>
class CollisionManager final {
private:

	Cowherd* cowherd_;
	YoungPerson* youngPerson_;
	MapChip* mapChip_;
	Cow* cow_;
	Dog* dog_;

	CowCollision* cowCollision_;
	CowherdCollision* cowherdCollison_;

public:
	// Constructor & Destructor
	CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog,
		CowCollision* cowCollision, CowherdCollision* cowherdCollision);
	~CollisionManager();

	// default method
	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog,
		CowCollision* cowCollision, CowherdCollision* cowherdCollision);
	//void Update();
	//void Draw();
	void Finalize();

	// user method
	void CheckCanMove();

	void CheckOverLapping();

	// 牛の動く方向の判定
	void CheckCanCowMove();

	// クリア条件の判定
	bool CheckClear();

private:

	// user method
	void CowherdCanMove();
	bool CowherdCheckCanMove(const Vec2& add);

	void YoungPersonCanMove();
	bool YoungPersonCheckCanMove(const Vec2& add);

	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

};


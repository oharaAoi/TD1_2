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

public:
	// Constructor & Destructor
	CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog);
	~CollisionManager();

	// default method
	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog);
	//void Update();
	//void Draw();
	void Finalize();

	// user method
	void CheckCanMove();

	void CheckCowMoveDire();

	void CheckOverLapping();

	// クリア条件の判定
	bool CheckClear();

private:

	// user method
	void CowherdCanMove();
	bool CowherdCheckCanMove(const Vec2& add);

	void YoungPersonCanMove();
	bool YoungPersonCheckCanMove(const Vec2& add);

	// 評価で使う
	void CheckCowAdjoin();
	void CheckDogExist();
	void CheckCowFourArea();
	void CheckGridDistance(const Vec2& add);
	void CheckCowMoveAllDire();

	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

};


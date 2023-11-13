#pragma once

#include "MyVector2.h"
#include <stack>
#include <queue>


enum kStackType {
	kStackType_Cowherd,
	kStackType_YoungPerson,
	kStackType_Dog
};

class Stack {
private:

	// 
	//std::queue<std::vector<std::vector<int>>> queueDate_;
	static std::stack<std::vector<std::vector<int>>> stackDate_;
	/*static std::stack<int> yp_index_;
	static std::stack<int> ch_isMovingCount_;*/

	//static std::stack<StackDate> stackDate_;

public:
	virtual ~Stack();

	static void PushDate(std::vector<std::vector<int>> mapAdd);
	//static void PushIndex(int yi);
	//static void PushCH_isMovingCount(int ch_isMovingCount);

	/*static void IndexPop();
	static void Ch_isMovingCountPop();*/

	static bool GetEmpty() { return stackDate_.empty(); }
	static std::vector<std::vector<int>> GetMapAdd() { return stackDate_.top(); }
	static void StackPop();

	/*static int GetIndex() { return yp_index_.top(); }
	static bool GetIndexEmpty() { return yp_index_.empty(); }

	static bool GetCH_IsMovingCountEmpty() { return ch_isMovingCount_.empty(); }*/

};


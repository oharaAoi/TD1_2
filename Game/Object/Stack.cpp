#include "Stack.h"

std::stack<std::vector<std::vector<int>>> Stack::stackDate_;
std::stack<int> Stack::yp_index_;
std::stack<int> Stack::ch_isMovingCount_;

Stack::~Stack() {}



void Stack::PushDate(std::vector<std::vector<int>> mapAdd) {
	stackDate_.push(mapAdd);
}

void Stack::PushIndex(int yi) {
	yp_index_.push(yi);
}

void Stack::PushCH_isMovingCount(int ch_isMovingCount) {
	ch_isMovingCount_.push(ch_isMovingCount);
}

void Stack::IndexPop() {
	yp_index_.pop();
}

void Stack::StackPop() {
	stackDate_.pop();
}

void Stack::Ch_isMovingCountPop() {
	ch_isMovingCount_.pop();
}
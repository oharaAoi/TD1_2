﻿#pragma once

// 移動できる方向を所得するときに使う
enum kCanMoveDirection {
	top,
	bottom,
	left,
	right,
	leftTop,
	rightTop,
	leftBottom,
	rightBottom
};

static const char* kCanMoveDire[8] = {
	"top",
	"bottom",
	"left",
	"right",
	"leftTop",
	"rightTop",
	"leftBottom",
	"rightBottom"
};
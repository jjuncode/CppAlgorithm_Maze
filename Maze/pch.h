#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include <stack>
#include "types.h"
#include <queue>
#include <map>

using namespace std;

struct Pos
{
	bool operator == (Pos& other)
	{
		return y == other.y && x == other.x;
	}

	bool operator != (Pos& other)
	{
		return !(*this == other);
	}

	Pos& operator += (Pos& other)
	{
		y += other.y;
		x += other.x;
		return *this;
	}

	Pos operator + (Pos& other)
	{
		Pos temp;
		temp.x = x + other.x;
		temp.y = y + other.y;
		return temp;
	}

	bool operator < (const Pos& other) const
	{
		if (y != other.y)
			return y < other.y;
		return x < other.x;
	}

	int32 y = 0;
	int32 x = 0;

};

enum Dir : int32
{
	UP = 0,
	RIGHT,
	DOWN,
	LEFT,

	END
};
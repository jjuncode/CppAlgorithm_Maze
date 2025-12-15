#include "pch.h"
#include "Player.h"
#include "Board.h"
#include "thread"

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;

	Pos dest = board->GetExitPos();
	Pos pos = _pos;
	_dir = Dir::DOWN;

	Pos MovePos[Dir::END]
	{
		{ -1, 0 }, // UP
		{ 0, 1 },  // RIGHT
		{ 1, 0 },  // DOWN
		{ 0, -1 }  // LEFT
	};

	_path.reserve(BOARD_MAX_SIZE);

	while (pos != dest)
	{
		// 현재 방향을 기준으로 우측이 갈 수 있는지 체크
		// 우측이 갈 수 있으면 우측으로 회전 후, 1칸 전진
		// 우측이 갈 수 없으면, 현재 방향으로 1칸 전진
		// 둘다 안되면 좌측 확인
		// 다 안되면 뒤로감
		
		auto dir_right = (_dir + 1) % Dir::END;
		auto dir_left = (_dir + 3) % Dir::END;

		// check right
		if (CanGo(pos + MovePos[dir_right]))
		{
			_dir = dir_right;
			pos = pos + MovePos[_dir];

		}
		// Check Forward
		else if (CanGo(pos + MovePos[_dir]))
		{
			pos = pos + MovePos[_dir];
		}
		// Go Left
		else if ( CanGo(pos + MovePos[dir_left]))
		{
			_dir = dir_left;
			pos = pos + MovePos[_dir];
		}
		// Go back
		else
		{
			auto dir_back = (_dir + 2) % Dir::END;
			_dir = dir_back;
			pos = pos + MovePos[_dir];
		}

		// Save Path
		_path.push_back(pos);
	}

	stack<Pos> stack;
	
	// 경로를 따라감
	// 경로를 갔는데, stack에 있는 경로라면 중복이라 pop ( 2칸차이 ) 
	for (int i = 0; i < _path.size()-1; ++i)
	{
		if (stack.empty() == false && stack.top() == _path[i + 1])
			stack.pop();
		else
			stack.push(_path[i]);
	}

	// 목적지 도착
	if (_path.empty() == false)
		stack.push(_path.back());

	vector<Pos> path;
	while (stack.empty() == false)
	{
		path.push_back(stack.top());
		stack.pop();
	}

	std::reverse(path.begin(), path.end());

	_path = path;
}

void Player::Update(uint64 deltaTick)
{
	if (idx < _path.size())
	{
		_pos = _path[idx];
		++idx;

		this_thread::sleep_for(1ms);
	}
}

bool Player::CanGo(Pos pos)
{
	auto type = _board->GetTileType(pos);
	if ( type == TileType::EMPTY ) 
		return true;

	return false;
}

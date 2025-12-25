#include "pch.h"
#include "Player.h"
#include "Board.h"
#include "thread"

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;

	//BFS();
	AStar();
}

void Player::Update(uint64 deltaTick)
{
	if (idx >= _path.size()) 
	{
		_board->GenerateMap();
		Init(_board);
	}


	if (idx < _path.size())
	{
		_pos = _path[idx];
		++idx;

		this_thread::sleep_for(100ms);
	}
}

bool Player::CanGo(Pos pos)
{
	auto type = _board->GetTileType(pos);
	if ( type == TileType::EMPTY ) 
		return true;

	return false;
}

void Player::RightHand()
{
	Pos dest = _board->GetExitPos();
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
		else if (CanGo(pos + MovePos[dir_left]))
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
	for (int i = 0; i < _path.size() - 1; ++i)
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

void Player::BFS()
{
	Pos dest = _board->GetExitPos();
	Pos pos = _pos;
	_dir = Dir::DOWN;

	Pos MovePos[Dir::END]
	{
		{ -1, 0 }, // UP
		{ 0, 1 },  // RIGHT
		{ 1, 0 },  // DOWN
		{ 0, -1 }  // LEFT
	};

	const int32 size = _board->GetSize();
	vector<vector<bool>> discovered(size, vector<bool>(size, false));

	queue<Pos> q;
	q.push(pos);
	discovered[pos.y][pos.x] = true;

	map<Pos, Pos> parent;	
	parent[pos] = pos;	// 시작점

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();

		if (pos == dest)
			break;

		for (int32 dir = 0; dir < 4; ++dir)
		{
			auto newPos = pos + MovePos[dir];
			if (CanGo(newPos) == false)
				continue;

			if (discovered[newPos.y][newPos.x] == false)
			{
				q.push(newPos);
				discovered[newPos.y][newPos.x] = true;
				parent[newPos] = pos;
			}
		}
	}

	_path.clear();

	Pos start = dest;
	Pos end = _board->GetEnterPos();

	while (true)
	{
		_path.push_back(start);

		if (start == end)
			break;

		start = parent[start];
	}

	std::reverse(_path.begin(), _path.end());
}

struct PQNode
{
	bool operator < (const PQNode& other) const { return f < other.f; }
	bool operator > (const PQNode& other) const { return f > other.f; }

	int32 f;
	int32 g;
	Pos pos;
};

void Player::AStar()
{
	Pos start = _pos;
	Pos dest = _board->GetExitPos();
	_dir = Dir::DOWN;

	enum
	{
		DIR_COUNT = 8
	};

	Pos MovePos[]
	{
		{ -1, 0 }, // UP
		{ 0, 1 },  // RIGHT
		{ 1, 0 },  // DOWN
		{ 0, -1 },  // LEFT
		{ -1, 1 }, // UP_RIGHT
		{ 1, 1 },  // RIGHT_DOWN
		{ 1, -1 },  // DOWN_LEFT
		{ -1, -1 }  // LEFT_UP
	};

	int32 cost[] =
	{
		10,
		10,
		10,
		10,
		14,
		14,
		14,
		14
	};

	const int32 size = _board->GetSize();

	vector<vector<int32>> best_cost(size, vector<int32>(size, INT32_MAX));
	map<Pos, Pos> parent;

	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

	{
		int32 g = 0;
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));
		pq.push(PQNode{ g + h, g, start });
		best_cost[start.y][start.x] = g + h;
		parent[start] = start;
	}

	while (pq.empty() == false)
	{
		PQNode node = pq.top();
		pq.pop();

		if (best_cost[node.pos.y][node.pos.x] < node.f)
			continue;

		if (node.pos == dest)
			break;

		for (int32 dir = 0; dir < DIR_COUNT; ++dir)
		{
			Pos nextPos = node.pos + MovePos[dir];

			if (CanGo(nextPos) == false)
				continue;

			// 이미 방문한 곳이면 스킵하도록 코드 추가해도됨 

			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));

			if (best_cost[nextPos.y][nextPos.x] <= g + h)
				continue;

			best_cost[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h,  g, nextPos });
			parent[nextPos] = node.pos;
		}
	}


	_path.clear();
	idx = 0;

	Pos s = dest;
	Pos e = _board->GetEnterPos();

	while (true)
	{
		_path.push_back(s);

		if (s == e)
			break;

		s = parent[s];
	}

	std::reverse(_path.begin(), _path.end());
}

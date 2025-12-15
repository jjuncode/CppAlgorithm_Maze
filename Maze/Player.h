#pragma once

class Board;

class Player
{
public:
	void	Init(Board* board);
	void	Update(uint64 deltaTick);

	void	SetPos(Pos pos) { _pos = pos; }
	Pos		GetPos() {return _pos;}

	bool CanGo(Pos pos);

private:
	Pos		_pos{};
	int32	_dir = Dir::END;	
	Board*	_board = nullptr;

	std::vector<Pos> _path;
	int32 idx = 0;
};


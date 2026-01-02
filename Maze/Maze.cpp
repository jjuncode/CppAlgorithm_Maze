#include "pch.h"
//#include <iostream>
//#include "Board.h"
//#include "Player.h"
//
//Board board;
//Player player;
//
//int main()
//{
//	::srand(static_cast<uint32>(time(nullptr)));
//	board.Init(25, &player);
//	player.Init(&board);
//
//	uint64 lastTick = 0;
//	while (true)
//	{
//		const uint64 currentTick = ::GetTickCount64();
//		const uint64 deltaTick = currentTick - lastTick;
//		lastTick = currentTick;
//
//		player.Update(deltaTick);
//		board.Render();
//	}
//}

#include "Sorting.h"
int main()
{
    vector<int> v{ 1,65,43,1,2,5,78,23,234,5,12 };
    MergeSort(v, 0, v.size() - 1);

    for (auto& _v : v)
    {
        cout << _v << " ";
    }
    cout << endl;
}

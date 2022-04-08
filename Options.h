#ifndef Options_
#define Options_

#include "Map.h"

//using minedreams::Player;

namespace minedreams
{
	class Options
	{
	public:
		void start();
		~Options();
	private:
		//地图大小
		static constexpr unsigned int MAP_SIZE = 0;
		//玩家列表
		static constexpr unsigned int PLAYER_LIST = MAP_SIZE + 1;
		//继续
		static unsigned int NEXT;
		//上
		static constexpr unsigned int UP = 0;
		//下
		static constexpr unsigned int DOWN = UP + 1;
		//左
		static constexpr unsigned int LEFT = DOWN + 1;
		//右
		static constexpr unsigned int RIGHT = LEFT + 1;

		//打印
		void print();
		//

		//存储光标位置
		int location[2] = { 0, -1 };
		//地图大小
		int size = 10;
		//玩家
		std::vector<Player*> players = std::vector<Player*>();
		//地图
		Map* map;
};
}

#endif Options_
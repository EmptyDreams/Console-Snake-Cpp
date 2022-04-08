#ifndef Map_
#define Map_

#include "Player.h"

namespace minedreams
{
	class MapPrinter;

	class Map
	{
		//使MapPrinter类成为友元，为了让MapPrinter能够访问Map类的私有属性
		//friend class MapPrinter;
	public:
		//方块
		typedef string Block;
		//地图边界
		static const char BLOCK_WALL = '#';
		//苹果
		static const Block BLOCK_APPLE;
		//毒苹果
		static const Block BLOCK_BAD_APPLE;

		//默认构造函数
		Map(const int size) : map(init(size)), SIZE(size), X_SIZE(size * 2) {}
		~Map();

		//添加玩家到地图
		void add_player(Player & player);
		//删除一名玩家
		inline void delete_player(Player & player)
			//查找玩家->删除
		{ players.erase(std::find(players.begin(), players.end(), &player)); }
		//更新地图，一般由MapPrinter调用
		void update_();
		//
		void update();
		//清理地图中央，一般由update方法调用
		void clear();
		//获取一个方块
		const string& get(int x, int y) const;
		//开始
		void start();
		//暂停/开始
		inline void pause() { is_stop = !is_stop; }
		//获取苹果坐标
		const location& get_apple_location() const { return apple_location; }
		//获取毒苹果坐标
		const location& get_bad_apple_location() const { return bad_apple_location; }
		//设置苹果方块
		void set_apple_block(const Block & block);
		//设置毒苹果方块
		void set_bad_apple_block(const Block & block);
		//生成苹果
		void creat_apple();
		//生成毒苹果
		void creat_bad_apple();
		//判断某个方块是否为空
		const bool is_null(int x, int y) const;

		//生成随机数
		int random();
	private:
		//设置方块
		void set(int x, int y, Block & b);
		//更新某个玩家的地图
		void update_(const Player & player);

		//毒苹果坐标
		location bad_apple_location = { 0, 0, Player::AIR };
		//苹果坐标
		location apple_location = bad_apple_location;
		//是否暂停
		bool is_stop = false;
		//生成缺省地图
		static string** init(const int & size);
		//保存地图
		string** map;
		//地图大小
		const int SIZE;
		//地图大小（x轴方向）
		const int X_SIZE;
		//存储玩家
		std::vector<Player*> players = std::vector<Player*>();
		//正在使用的苹果方块
		Block using_apple = BLOCK_APPLE;
		//正在使用的毒苹果方块
		Block using_bad_apple = BLOCK_BAD_APPLE;
	};
}

#endif Map_
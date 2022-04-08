#ifndef Player_
#define Player_

#include <vector>

namespace minedreams
{

	class Map;

	class Player
	{
	public:
		friend class Map;
		friend class MapPrinter;

		struct key
		{
			int move_up;
			int move_down;
			int move_right;
			int move_left;
		};

		//构造函数
		 Player(const string & name) :NAME(name) {}
		~Player();

		//复制
		Player(const Player & player) = delete;
		Player operator= (const Player & player) = delete;
		friend inline bool operator== (const Player & p1, const Player & p2);

		//默认身体方块
		static const string SRC_BODY;
		//默认头部方块
		static const string SRC_HEAD;
		//向上移动
		static constexpr int MOVE_UP = 1;
		//向下移动
		static constexpr int MOVE_DOWN = -MOVE_UP;
		//向右移动
		static constexpr int MOVE_RIGHT = 2;
		//向左移动
		static constexpr int MOVE_LEFT = -MOVE_RIGHT;

		//设置头部方块
		//	抛出：
		//		range_error - 如果字符串长度大于2，实际上字符串长度不应大于1，
		//			但是由于特殊字符占用两个长度，所以内部检测时长度不应大于2
		void set_head_block(string & head_block);
		//设置身体方块
		//	抛出：
		//		range_error - 如果字符串长度大于2，实际上字符串长度不应大于1，
		//			但是由于特殊字符占用两个长度，所以内部检测时长度不应大于2
		void set_body_block(string & body_block);
		//长度加一
		void plus_length();
		//长度减一
		void minus_length();
		//积分加减
		void plus_score(int amount = 5);
		//移动
		bool move(const int direction);
		//设置移动方向
		void set_direction(int direction);

		//宣告玩家死亡
		void die() { is_live_ = false; }
		//判断玩家是否存活
		const bool is_live() const { return is_live_; }
		//移动
		bool move() { return move(move_direction); }
		//迭代器-begin
		const auto begin() const { return bodys.begin(); }
		//迭代器-end
		const auto end() const { return bodys.end(); }
		//获取头部坐标
		const location* get_head_location() const { return head_location; }
		//设置快捷键
		void set_key(const key & key) { move_key = key; }
		//获取快捷键
		const key& get_key() const { return move_key; }
		//获取长度
		const unsigned int get_length() const { return length; }
		//获取积分
		const unsigned int get_score() const { return score; }
		//获取名称
		const string& get_name() const { return NAME; }
		//更改名称
		void set_name(const string & name) { NAME = name; }
	private:
		//空的
		static string AIR;
		//内部移动
		bool move_(const int x, const int y);

		//玩家名称
		string NAME;
		//玩家长度
		unsigned int length = 1;
		//玩家积分
		unsigned long long score = 0;
		//食用毒苹果数量
		unsigned long long eat_bad_apple = 0;
		//食用苹果数量
		unsigned long long eat_apple = 0;
		//玩家是否存活
		bool is_live_ = true;
		//正在使用的身体方块
		string body_block = SRC_BODY;
		//正在使用的头部方块
		string head_block = SRC_HEAD;
		//头部坐标
		location* head_location = new location{ -1, -1, head_block };
		//身体坐标
		std::vector<location*> bodys = std::vector<location*>();
		//尾坐标
		location ll = { -1, -1, AIR };
		//移动方向
		int move_direction = MOVE_RIGHT;
		//移动方向（缓存，为了消除反向移动BUG）
		int move_direction_cache = move_direction;
		//快捷键
		key move_key = { 72, 80, 77, 75 };
	};

	inline bool operator== (const Player & p1, const Player & p2) { return &p1 == &p2; }

}

#endif Player_
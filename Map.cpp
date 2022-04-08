#include "pch.h"
#include "Map.h"
#include <random>
#include <conio.h>

using minedreams::Map;
using minedreams::Player;

const Map::Block Map::BLOCK_APPLE = "⊕";
const Map::Block Map::BLOCK_BAD_APPLE = "⊙";

int Map::random()
{
	static std::default_random_engine e;
	static std::random_device rd;
	std::uniform_int_distribution<unsigned> u(0, SIZE - 3);
	e.seed(rd());
	return u(e);
}

void Map::add_player(Player & player)
{
	players.insert(players.end(), &player);
	if (player.length != 1)
		throw std::out_of_range("玩家长度应该为1");
	location l{ random(), random(), Player::AIR };
	while (true)
	{
		if (get(l.x, l.y) != " ")
		{
			l.x = random();
			l.y = random();
		}
		else
			break;
	}
	player.head_location->x = l.x;
	player.head_location->y = l.y;
}

string** Map::init(const int & size)
{
	int x_size = size * 2;
	//分配内存
	string** c = new string*[size];
	for (int i = 0; i < size; ++i)
		c[i] = new string[x_size];
	//写入基本数据
	for (int i = 0; i < size; ++i)
		for (int k = 0; k < x_size; ++k)
			//若坐标在边框则填充为BLOCK_WALL，否则填充为空格
			c[i][k] = (i == 0 || i == size - 1) ?
			"#" : (k == 0 || k == x_size - 1) ? "#" : " ";
	return c;
}

static bool init_(const HANDLE & H, const HANDLE & H2)
{
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(H, &cci);
	SetConsoleCursorInfo(H2, &cci);
	return true;
}

void Map::update()
{
	//默认显示
	static HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	//缓冲
	static HANDLE H2 = GetStdHandle(STD_ERROR_HANDLE);
	static HANDLE HS[2] = { H, H2 };
	//初始化
	static const bool v = init_(H, H2);
	//输出端
	static int index = 0;

	//字数
	static DWORD bytes = 0;
	COORD coord = { 0,0 };

	string* maps = new string[SIZE];
	bool b;
	//将string**类型的地图转换为string*格式
	for (int i = 0; i < SIZE; ++i)
	{
		b = false;
		maps[i] = string();
		for (int k = 0; k < X_SIZE; ++k)
		{
			if (b)
			{
				b = false;
				continue;
			}
			if (map[i][k].length() == 2)
				b = true;
			string & c = map[i][k];
			maps[i].append(map[i][k]);
		}
	}
	//打印地图
	for (int i = 0; i < SIZE; ++i)
	{
		coord.Y = i;
		const char* c = maps[i].c_str();
		WriteConsoleOutputCharacterA(HS[index], maps[i].c_str(), maps[i].length(), coord, &bytes);
	}

	static int player_index = 0;
	static string up;
	string s = "玩家名称：" + players[player_index++]->NAME;
	if (player_index == players.size()) player_index = 0;
	coord.Y = 0;
	coord.X = X_SIZE + 5;
	if (s.size() < up.size())
		FillConsoleOutputCharacter(HS[index], ' ', up.size(), coord, &bytes);
	WriteConsoleOutputCharacter(HS[index], s.c_str(), s.size(), coord, &bytes);
	//更新下标，若下标为0则便为1，否则为0
	index = index == 0 ? 1 : 0;
	up = s;
}

void Map::update_()
{
	clear();
	for (const auto & p : players)
		update_(*p);
}

void Map::update_(const Player & player)
{
	if (!player.is_live()) return;
	const location* pl = player.get_head_location();
	set(pl->x, pl->y, pl->type);
	for (const location* L : player)
		set(L->x, L->y, L->type);
}

void Map::clear()
{
	string air(" ");
	for (int i = 0; i < SIZE - 1; ++i)
		for (int k = 0; k < SIZE - 2; ++k)
			set(i, k, air);
}

const string& Map::get(int x, int y) const
{
	if (x >= X_SIZE - 2 || y >= SIZE - 2 || y < 0 || x < 0)
		throw std::out_of_range("坐标超出地图范围");
	return map[++y][++x];
}

void Map::set(int x, int y, Block & b)
{
	if (x > SIZE - 2 || y > SIZE - 2 || y < 0 || x < 0)
		throw std::out_of_range("坐标超出地图范围");
	//因为这里的执行顺序为从右到左，所以在右边对y进行自加操作
	x *= 2;
	map[y][++x] = map[++y][++x] = b;
}

void Map::start()
{
	//存储线程
	static std::vector<thread*> ts = std::vector<thread*>();
	//移动玩家
	//为了节省线程所以所有玩家的移动操作写在一个线程中
	int is = players.size();
	ts.push_back(new thread([&]() {
		double start = clock();
		double time = 400;
		double t;
		while (is > 0)
		{
			//while (clock() - start < 400);
			t = 400 - (clock() - start);
			Sleep(400);
			start = clock();
			bool b[2] = { false, false };
			//遍历玩家
			for (Player* p : players)
			{
				//如果游戏暂停则跳过循环
				if (is_stop) continue;
				//如果玩家死亡则跳过循环
				if (!p->is_live()) continue;
				//更新移动方向
				p->move_direction = p->move_direction_cache;
				//move()方法返回是否死亡但是检测不完整
				if (!p->move())
				{
					--is;
					continue;
				}
				//检查玩家是否超出地图边界
				if (p->get_head_location()->x > SIZE - 2 ||
					p->get_head_location()->y >= SIZE - 2)
				{
					//如果超出地图边界则宣告死亡并且跳过剩余循环
					p->die();
					--is;
					continue;
				}
				//判断玩家是否吃到苹果
				const location & l = *p->get_head_location();
				const location & l2 = get_apple_location();
				if (l.x == l2.x && l.y == l2.y)
				{
					p->plus_length();
					p->plus_score();
					b[0] = true;
				}
				//判断玩家是否吃到毒苹果
				const location & l3 = get_bad_apple_location();
				if (l.x == l3.x && l.y == l3.y)
				{
					b[1] = true;
					if (p->get_length() == 1)
					{
						p->die();
						--is;
						if (p->get_score() <= 2)
							p->plus_score(-((int)p->get_score()));
						else
							p->plus_score();
						continue;
					}
					p->minus_length();
					p->plus_score(-2);
				}
			}	//end for
			update_();				//更新地图内容
			if (b[0])
				creat_apple();		//如果玩家吃到了苹果则重新生成一个
			if (b[1])
				creat_bad_apple();
			//将苹果添加到地图中
			set(apple_location.x, apple_location.y, using_apple);
			set(bad_apple_location.x, bad_apple_location.y, using_bad_apple);
			update();				//更新显示
		}
	}));

	//因为_kbhit()方法比较占用CPU，这里把所有玩家的按键监听放在一个线程
	ts.push_back(new thread([&]()
	{
		while (is > 0)
		{
			if (_kbhit())
			{
				int ch = _getch();
				if (ch == 32) pause();
				else if (!is_stop)
				{
					for (auto p : players)
					{
						if (ch == p->move_key.move_down)
							p->set_direction(Player::MOVE_DOWN);
						else if (ch == p->move_key.move_left)
							p->set_direction(Player::MOVE_LEFT);
						else if (ch == p->move_key.move_right)
							p->set_direction(Player::MOVE_RIGHT);
						else if (ch == p->move_key.move_up)
							p->set_direction(Player::MOVE_UP);
					}	//end for
				}	//end if
			}	//end if
			Sleep(10);		//添加延迟来降低CPU占用
		}	//end while
	})); //end thread
	
	creat_apple();		//生成地图最初的苹果和毒苹果
	creat_bad_apple();

	//等待线程进行完毕并释放资源
	//等待线程结束是为了避免程序在线程结束之前结束而导致的报错
	for (auto t : ts)
	{
		t->join();
		delete t;
	}
	ts.clear();
}

void Map::set_apple_block(const Block & block)
{
	if (block != using_apple)
		using_apple = block;
}

void Map::set_bad_apple_block(const Block & block)
{
	if (block != using_bad_apple)
		using_bad_apple = block;
}

static string air;

void Map::creat_bad_apple()
{
	location l = { 0, 0, air };
	while (true)
	{
		l.x = random();
		l.y = random();
		if (is_null(l.x, l.y)) break;
	}
	bad_apple_location = l;
}

void Map::creat_apple()
{
	location l = { 0, 0, air };
	while (true)
	{
		l.x = random();
		l.y = random();
		if (is_null(l.x, l.y)) break;
	}
	apple_location = l;
}

const bool Map::is_null(int x, int y) const
{
	if ((bad_apple_location.x == x && bad_apple_location.y == y) ||
		(apple_location.x == x && apple_location.y == y))
		return false;
	for (auto p : players)
	{
		if (x == p->head_location->x && y == p->head_location->y)
			return false;
		for (auto l : *p)
		{
			if (x == l->x && y == l->y)
				return false;
		}
	}
	return true;
}

Map::~Map()
{
	for (int i = 0; i < SIZE; ++i)
		delete[] map[i];
	delete[] map;
}

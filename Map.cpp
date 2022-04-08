#include "pch.h"
#include "Map.h"
#include <random>
#include <conio.h>

using minedreams::Map;
using minedreams::Player;

const Map::Block Map::BLOCK_APPLE = "��";
const Map::Block Map::BLOCK_BAD_APPLE = "��";

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
		throw std::out_of_range("��ҳ���Ӧ��Ϊ1");
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
	//�����ڴ�
	string** c = new string*[size];
	for (int i = 0; i < size; ++i)
		c[i] = new string[x_size];
	//д���������
	for (int i = 0; i < size; ++i)
		for (int k = 0; k < x_size; ++k)
			//�������ڱ߿������ΪBLOCK_WALL���������Ϊ�ո�
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
	//Ĭ����ʾ
	static HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	//����
	static HANDLE H2 = GetStdHandle(STD_ERROR_HANDLE);
	static HANDLE HS[2] = { H, H2 };
	//��ʼ��
	static const bool v = init_(H, H2);
	//�����
	static int index = 0;

	//����
	static DWORD bytes = 0;
	COORD coord = { 0,0 };

	string* maps = new string[SIZE];
	bool b;
	//��string**���͵ĵ�ͼת��Ϊstring*��ʽ
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
	//��ӡ��ͼ
	for (int i = 0; i < SIZE; ++i)
	{
		coord.Y = i;
		const char* c = maps[i].c_str();
		WriteConsoleOutputCharacterA(HS[index], maps[i].c_str(), maps[i].length(), coord, &bytes);
	}

	static int player_index = 0;
	static string up;
	string s = "������ƣ�" + players[player_index++]->NAME;
	if (player_index == players.size()) player_index = 0;
	coord.Y = 0;
	coord.X = X_SIZE + 5;
	if (s.size() < up.size())
		FillConsoleOutputCharacter(HS[index], ' ', up.size(), coord, &bytes);
	WriteConsoleOutputCharacter(HS[index], s.c_str(), s.size(), coord, &bytes);
	//�����±꣬���±�Ϊ0���Ϊ1������Ϊ0
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
		throw std::out_of_range("���곬����ͼ��Χ");
	return map[++y][++x];
}

void Map::set(int x, int y, Block & b)
{
	if (x > SIZE - 2 || y > SIZE - 2 || y < 0 || x < 0)
		throw std::out_of_range("���곬����ͼ��Χ");
	//��Ϊ�����ִ��˳��Ϊ���ҵ����������ұ߶�y�����ԼӲ���
	x *= 2;
	map[y][++x] = map[++y][++x] = b;
}

void Map::start()
{
	//�洢�߳�
	static std::vector<thread*> ts = std::vector<thread*>();
	//�ƶ����
	//Ϊ�˽�ʡ�߳�����������ҵ��ƶ�����д��һ���߳���
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
			//�������
			for (Player* p : players)
			{
				//�����Ϸ��ͣ������ѭ��
				if (is_stop) continue;
				//����������������ѭ��
				if (!p->is_live()) continue;
				//�����ƶ�����
				p->move_direction = p->move_direction_cache;
				//move()���������Ƿ��������Ǽ�ⲻ����
				if (!p->move())
				{
					--is;
					continue;
				}
				//�������Ƿ񳬳���ͼ�߽�
				if (p->get_head_location()->x > SIZE - 2 ||
					p->get_head_location()->y >= SIZE - 2)
				{
					//���������ͼ�߽�������������������ʣ��ѭ��
					p->die();
					--is;
					continue;
				}
				//�ж�����Ƿ�Ե�ƻ��
				const location & l = *p->get_head_location();
				const location & l2 = get_apple_location();
				if (l.x == l2.x && l.y == l2.y)
				{
					p->plus_length();
					p->plus_score();
					b[0] = true;
				}
				//�ж�����Ƿ�Ե���ƻ��
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
			update_();				//���µ�ͼ����
			if (b[0])
				creat_apple();		//�����ҳԵ���ƻ������������һ��
			if (b[1])
				creat_bad_apple();
			//��ƻ����ӵ���ͼ��
			set(apple_location.x, apple_location.y, using_apple);
			set(bad_apple_location.x, bad_apple_location.y, using_bad_apple);
			update();				//������ʾ
		}
	}));

	//��Ϊ_kbhit()�����Ƚ�ռ��CPU�������������ҵİ�����������һ���߳�
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
			Sleep(10);		//����ӳ�������CPUռ��
		}	//end while
	})); //end thread
	
	creat_apple();		//���ɵ�ͼ�����ƻ���Ͷ�ƻ��
	creat_bad_apple();

	//�ȴ��߳̽�����ϲ��ͷ���Դ
	//�ȴ��߳̽�����Ϊ�˱���������߳̽���֮ǰ���������µı���
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

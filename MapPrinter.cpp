#include "pch.h"
/*
#include "MapPrinter.h"
#include <conio.h>

using minedreams::MapPrinter;

static bool init(const HANDLE & H, const HANDLE & H2)
{
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(H, &cci);
	SetConsoleCursorInfo(H2, &cci);
	return true;
}

void MapPrinter::update() const
{
	//默认显示
	static HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	//缓冲
	static HANDLE H2 = GetStdHandle(STD_ERROR_HANDLE);
	static HANDLE HS[2] = { H, H2 };
	//初始化
	static const bool & v = init(H, H2);
	//输出端
	static int index = 0;

	//字数
	static DWORD bytes = 0;
	static COORD coord = { 0,0 };

	map.update();

	string* maps = new string[map.SIZE];
	bool b;
	for (int i = 0; i < map.SIZE; ++i)
	{
		b = false;
		maps[i] = string();
		for (int k = 0; k < map.X_SIZE; ++k)
		{
			if (b)
			{
				b = false;
				continue;
			}
			if (map.map[i][k].length() == 2)
				b = true;
			string & c = map.map[i][k];
			maps[i].append(map.map[i][k]);
		}
	}
	
	for (int i = 0; i < map.SIZE; ++i)
	{
		coord.Y = i;
		const char* c = maps[i].c_str();
		WriteConsoleOutputCharacterA(HS[index], maps[i].c_str(), maps[i].length(), coord, &bytes);
	}
	++coord.Y;
	WriteConsoleOutputCharacterA(HS[index], std::to_string(map.players[0]->get_length()).c_str(),10, coord, &bytes);
	SetConsoleActiveScreenBuffer(HS[index]);
	index = index == 0 ? 1 : 0;

	delete[] maps;
}

void MapPrinter::start()
{
	static std::vector<thread*> ts = std::vector<thread*>();
	ts.push_back(new thread([&]() {
		while (map.players.size() > 0)
		{
			Sleep(500);
			for (Player* p : map.players)
			{
				if (map.is_stop) continue;
				if (!p->is_live()) continue;
				p->move_direction = p->move_direction_cache;
				if (!p->move()) continue;
				if (p->get_head_location()->x > map.SIZE - 2 ||
					p->get_head_location()->y >= map.SIZE - 2)
				{
					p->die();
					continue;
				}
				const location & l = *p->get_head_location();
				const location & l2 = map.get_apple_location();
				if (l.x == l2.x && l.y == l2.y)
				{
					p->plus_length();
					p->plus_score();
					map.creat_apple();
					continue;
				}
				const location & l3 = map.get_bad_apple_location();
				if (l.x == l3.x && l.y == l3.y)
				{
					p->minus_length();
					p->plus_score(-2);
					map.creat_bad_apple();
				}
			}
			update();
		}
	}));
	//map.creat_apple();
	//map.creat_bad_apple();
	for (Player* p : map.players)
	{
		Player* p0 = p;
		ts.push_back(new thread([&]() {
			while (p0->is_live())
			{
				if (_kbhit())
				{
					int ch = _getch();
					if (ch == p0->move_key.move_down && !map.is_stop)
						p0->set_direction(Player::MOVE_DOWN);
					else if (ch == p0->move_key.move_left && !map.is_stop)
						p0->set_direction(Player::MOVE_LEFT);
					else if (ch == p0->move_key.move_right && !map.is_stop)
						p0->set_direction(Player::MOVE_RIGHT);
					else if (ch == p0->move_key.move_up && !map.is_stop)
						p0->set_direction(Player::MOVE_UP);
					else if (ch == 32) map.pause();
				}
			}
		}));
	}
	for (auto t : ts)
	{
		t->join();
		delete t;
	}
	ts.clear();
}
*/
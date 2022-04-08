#include "pch.h"
#include "Options.h"
#include <iostream>
#include <conio.h>
#include <sstream>
#include <limits>  

using std::cout;
using minedreams::Options;

unsigned int Options::NEXT = PLAYER_LIST + 1;

//输入int
static bool cinInt(std::istream & cin, int & i)
{
	std::string c;
	std::getline(cin, c);
	std::stringstream is;
	is << c;
	if (is >> i)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//格子大小
const int width = 15;

void Options::print()
{
	using std::endl;

	//顶部边框
	const string wall = "+---------------+---------------+";
	const string wall2 = "-------+-------+-------+-------+";
	NEXT = players.size() + PLAYER_LIST + 1;

	system("cls");
	cout << wall << wall2 << endl;
	(cout << '|').width(width);
	(cout << "地图大小" << '|').width(width);

	(cout << size << '|').width(7);
	(cout << "上" << '|').width(7);
	(cout << "下" << '|').width(7);
	(cout << "左" << '|').width(7);
	(cout << "右" << '|').width(7);

	cout << endl << wall << wall2 << endl;
	(cout << '|').width(width);
	cout << "玩家列表" << '|';
	for (int i = 0; i < players.size(); ++i)
	{
		Player* p = players[i];
		cout.width(width);
		if (p->get_name().length() <= width)
			cout << p->get_name() << '|';
		else
		{
			cout.width(0);
			cout << p->get_name().substr(0, width - 3) << "..." << '|';
		}
		cout.width(7);
		char c[2] = { p->get_key().move_up, '\0' };
		cout << ((c[0] == 72) ? "↑" : c) << '|';
		c[0] = p->get_key().move_down;
		cout.width(7);
		cout << ((c[0] == 80) ? "↓" : c) << '|';
		c[0] = p->get_key().move_left;
		cout.width(7);
		cout << ((c[0] == 75) ? "←" : c) << '|';
		c[0] = p->get_key().move_right;
		cout.width(7);
		cout << ((c[0] == 77) ? "→" : c) << '|' << endl;

		cout << wall << wall2 << endl;
		(cout << '|').width(width);
		cout << ' ' << '|';
	}
	cout.width(width);
	cout << '+' << '|' << endl;
	(cout << wall << endl << '|').width(width);
	(cout << "继续" << '|').width(width);
	cout << "next" << '|' << endl << wall;
}

void Options::start()
{
	static HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	static COORD C = { 0, 0 };
	C.X = 17;
	static const WORD color = 0x70;
	static const WORD color2 = 0x07;
	print();
	while (true)
	{
		Sleep(10);
		NEXT = players.size() + PLAYER_LIST + 1;
		if (_kbhit())
		{
			int ck = _getch();
			switch (ck)
			{
			case 72:
				location[0] = location[0] == MAP_SIZE ? NEXT : --location[0];
				location[1] = -1;
				break;
			case 80:
				location[0] = location[0] == NEXT ? MAP_SIZE : ++location[0];
				location[1] = -1;
				break;
			case 75:
				if (location[0] == PLAYER_LIST)
					location[1] = location[1] == -1 ? RIGHT : --location[1];
				break;
			case 77:
				if (location[0] == PLAYER_LIST)
					location[1] = location[1] == RIGHT ? -1 : ++location[1];
				print();
				break;
			case 13:
				ck = -1;
				break;
			case 63:
				print();
				break;
			}
			C.Y = location[0] * 2 + 1;
			SetConsoleCursorPosition(H, C);
			if (location[0] == MAP_SIZE)
			{
			hear0:
				if (ck == -1)
				{
					SetConsoleTextAttribute(H, color);
					cout.width(width);
					cout << size;
					int i;
					SetConsoleCursorPosition(H, C);
					while (!cinInt(std::cin, i))
						SetConsoleCursorPosition(H, C);
					if (i < 8 || i > 50) goto hear0;
					size = i;
					SetConsoleCursorPosition(H, C);
					SetConsoleTextAttribute(H, color2);
					cout.width(width);
					cout << size;
					SetConsoleCursorPosition(H, C);
				}
			}
			else if (location[0] >= PLAYER_LIST && location[0] < NEXT)
			{
			hear1:
				if (location[1] == -1)
				{
					if (ck == -1)
					{
						SetConsoleTextAttribute(H, color);
						cout.width(width);
						cout << ' ';
						SetConsoleCursorPosition(H, C);
						string name;
						std::getline(std::cin, name);
						if (location[0] == NEXT - 1)
						{
							players.push_back(new Player(name));
							SetConsoleTextAttribute(H, color2);
							print();
						}
						else
						{
							int index = location[0] - PLAYER_LIST;
							players[index]->set_name(name);
							SetConsoleTextAttribute(H, color2);
							SetConsoleCursorPosition(H, C);
							cout.width(width);
							cout << name;
							SetConsoleCursorPosition(H, C);
						}
					}	//end if
				}
				else
				{
					if (location[0] == NEXT - 1)
					{
						location[1] = -1;
						goto hear1;
					}
					COORD co = C;
					co.X = co.X + width + (location[1] + 1) + 7 * location[1];
					SetConsoleCursorPosition(H, co);
					if (ck == -1)
					{
						int index = location[0] - PLAYER_LIST;
						Player & p = *players[index];
						Player::key k = p.get_key();
						SetConsoleTextAttribute(H, color);
						cout.width(7);
						switch (location[1])
						{
						case 0:
							cout << k.move_up;
							break;
						case 1:
							cout << k.move_down;
							break;
						case 2:
							cout << k.move_left;
							break;
						case 3:
							cout << k.move_right;
							break;
						}
					hear2:
						SetConsoleCursorPosition(H, co);
						int i;
						cinInt(std::cin, i);
						if (i < 0 || i >(std::numeric_limits<char>::max)()) goto hear2;
						SetConsoleCursorPosition(H, co);
						SetConsoleTextAttribute(H, color2);
						cout.width(7);
						cout << i;
						SetConsoleCursorPosition(H, co);
						switch (location[1])
						{
						case 0:
							k.move_up = i;
							break;
						case 1:
							k.move_down = i;
							break;
						case 2:
							k.move_left = i;
							break;
						case 3:
							k.move_right = i;
							break;
						}
						p.set_key(k);
					}
				}
			}
			else if (ck == -1) break;
		}	//end if
	}	//end while

	map = new Map(size);
	if (players.empty()) exit(0);
	for (auto p : players)
	{
		map->add_player(*p);
	}
	system("cls");
	map->start();
}

Options::~Options()
{
	for (auto p : players)
	{
		delete p;
	}
	delete map;
}
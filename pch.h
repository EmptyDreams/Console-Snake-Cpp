#ifndef PCH_H
#define PCH_H

#include <Windows.h>
#include <string>
#include <exception>
#include <thread>

using std::string;
using std::thread;

namespace minedreams
{
	//保存坐标
	struct location
	{
		int x;
		int y;
		string& type;

		inline location* operator=(const location & l)
		{
			x = l.x;
			y = l.y;
			type = l.type;
			return this;
		}
	};
}

#endif

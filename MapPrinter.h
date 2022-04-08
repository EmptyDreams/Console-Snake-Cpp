/*#ifndef MapPrinter_
#define MapPrinter_

#include "Map.h"

namespace minedreams
{
	//地图打印机，用来实现无闪烁打印地图
	//注意：
	//	1.此类在被析构时不会删除内部用于保存地图的二维数组，
	//		为了防止误删数据，删除数组的操作应当由用户自己完成
	//	2.update方法在更新显示前会自动更新地图内容，不需要手动更新地图
	class MapPrinter
	{
	public:
		//唯一的构造函数
		//参数：
		//	map - 地图
		MapPrinter(Map & map) :map(map) {}

		//更新输出
		void update() const;
		//开始
		void start();
	private:
		Map & map;
	};
}

#endif MapPrinter_
*/
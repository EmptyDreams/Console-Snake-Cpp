/*#ifndef MapPrinter_
#define MapPrinter_

#include "Map.h"

namespace minedreams
{
	//��ͼ��ӡ��������ʵ������˸��ӡ��ͼ
	//ע�⣺
	//	1.�����ڱ�����ʱ����ɾ���ڲ����ڱ����ͼ�Ķ�ά���飬
	//		Ϊ�˷�ֹ��ɾ���ݣ�ɾ������Ĳ���Ӧ�����û��Լ����
	//	2.update�����ڸ�����ʾǰ���Զ����µ�ͼ���ݣ�����Ҫ�ֶ����µ�ͼ
	class MapPrinter
	{
	public:
		//Ψһ�Ĺ��캯��
		//������
		//	map - ��ͼ
		MapPrinter(Map & map) :map(map) {}

		//�������
		void update() const;
		//��ʼ
		void start();
	private:
		Map & map;
	};
}

#endif MapPrinter_
*/
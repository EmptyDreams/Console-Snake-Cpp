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
		//��ͼ��С
		static constexpr unsigned int MAP_SIZE = 0;
		//����б�
		static constexpr unsigned int PLAYER_LIST = MAP_SIZE + 1;
		//����
		static unsigned int NEXT;
		//��
		static constexpr unsigned int UP = 0;
		//��
		static constexpr unsigned int DOWN = UP + 1;
		//��
		static constexpr unsigned int LEFT = DOWN + 1;
		//��
		static constexpr unsigned int RIGHT = LEFT + 1;

		//��ӡ
		void print();
		//

		//�洢���λ��
		int location[2] = { 0, -1 };
		//��ͼ��С
		int size = 10;
		//���
		std::vector<Player*> players = std::vector<Player*>();
		//��ͼ
		Map* map;
};
}

#endif Options_
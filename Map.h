#ifndef Map_
#define Map_

#include "Player.h"

namespace minedreams
{
	class MapPrinter;

	class Map
	{
		//ʹMapPrinter���Ϊ��Ԫ��Ϊ����MapPrinter�ܹ�����Map���˽������
		//friend class MapPrinter;
	public:
		//����
		typedef string Block;
		//��ͼ�߽�
		static const char BLOCK_WALL = '#';
		//ƻ��
		static const Block BLOCK_APPLE;
		//��ƻ��
		static const Block BLOCK_BAD_APPLE;

		//Ĭ�Ϲ��캯��
		Map(const int size) : map(init(size)), SIZE(size), X_SIZE(size * 2) {}
		~Map();

		//�����ҵ���ͼ
		void add_player(Player & player);
		//ɾ��һ�����
		inline void delete_player(Player & player)
			//�������->ɾ��
		{ players.erase(std::find(players.begin(), players.end(), &player)); }
		//���µ�ͼ��һ����MapPrinter����
		void update_();
		//
		void update();
		//�����ͼ���룬һ����update��������
		void clear();
		//��ȡһ������
		const string& get(int x, int y) const;
		//��ʼ
		void start();
		//��ͣ/��ʼ
		inline void pause() { is_stop = !is_stop; }
		//��ȡƻ������
		const location& get_apple_location() const { return apple_location; }
		//��ȡ��ƻ������
		const location& get_bad_apple_location() const { return bad_apple_location; }
		//����ƻ������
		void set_apple_block(const Block & block);
		//���ö�ƻ������
		void set_bad_apple_block(const Block & block);
		//����ƻ��
		void creat_apple();
		//���ɶ�ƻ��
		void creat_bad_apple();
		//�ж�ĳ�������Ƿ�Ϊ��
		const bool is_null(int x, int y) const;

		//���������
		int random();
	private:
		//���÷���
		void set(int x, int y, Block & b);
		//����ĳ����ҵĵ�ͼ
		void update_(const Player & player);

		//��ƻ������
		location bad_apple_location = { 0, 0, Player::AIR };
		//ƻ������
		location apple_location = bad_apple_location;
		//�Ƿ���ͣ
		bool is_stop = false;
		//����ȱʡ��ͼ
		static string** init(const int & size);
		//�����ͼ
		string** map;
		//��ͼ��С
		const int SIZE;
		//��ͼ��С��x�᷽��
		const int X_SIZE;
		//�洢���
		std::vector<Player*> players = std::vector<Player*>();
		//����ʹ�õ�ƻ������
		Block using_apple = BLOCK_APPLE;
		//����ʹ�õĶ�ƻ������
		Block using_bad_apple = BLOCK_BAD_APPLE;
	};
}

#endif Map_
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

		//���캯��
		 Player(const string & name) :NAME(name) {}
		~Player();

		//����
		Player(const Player & player) = delete;
		Player operator= (const Player & player) = delete;
		friend inline bool operator== (const Player & p1, const Player & p2);

		//Ĭ�����巽��
		static const string SRC_BODY;
		//Ĭ��ͷ������
		static const string SRC_HEAD;
		//�����ƶ�
		static constexpr int MOVE_UP = 1;
		//�����ƶ�
		static constexpr int MOVE_DOWN = -MOVE_UP;
		//�����ƶ�
		static constexpr int MOVE_RIGHT = 2;
		//�����ƶ�
		static constexpr int MOVE_LEFT = -MOVE_RIGHT;

		//����ͷ������
		//	�׳���
		//		range_error - ����ַ������ȴ���2��ʵ�����ַ������Ȳ�Ӧ����1��
		//			�������������ַ�ռ���������ȣ������ڲ����ʱ���Ȳ�Ӧ����2
		void set_head_block(string & head_block);
		//�������巽��
		//	�׳���
		//		range_error - ����ַ������ȴ���2��ʵ�����ַ������Ȳ�Ӧ����1��
		//			�������������ַ�ռ���������ȣ������ڲ����ʱ���Ȳ�Ӧ����2
		void set_body_block(string & body_block);
		//���ȼ�һ
		void plus_length();
		//���ȼ�һ
		void minus_length();
		//���ּӼ�
		void plus_score(int amount = 5);
		//�ƶ�
		bool move(const int direction);
		//�����ƶ�����
		void set_direction(int direction);

		//�����������
		void die() { is_live_ = false; }
		//�ж�����Ƿ���
		const bool is_live() const { return is_live_; }
		//�ƶ�
		bool move() { return move(move_direction); }
		//������-begin
		const auto begin() const { return bodys.begin(); }
		//������-end
		const auto end() const { return bodys.end(); }
		//��ȡͷ������
		const location* get_head_location() const { return head_location; }
		//���ÿ�ݼ�
		void set_key(const key & key) { move_key = key; }
		//��ȡ��ݼ�
		const key& get_key() const { return move_key; }
		//��ȡ����
		const unsigned int get_length() const { return length; }
		//��ȡ����
		const unsigned int get_score() const { return score; }
		//��ȡ����
		const string& get_name() const { return NAME; }
		//��������
		void set_name(const string & name) { NAME = name; }
	private:
		//�յ�
		static string AIR;
		//�ڲ��ƶ�
		bool move_(const int x, const int y);

		//�������
		string NAME;
		//��ҳ���
		unsigned int length = 1;
		//��һ���
		unsigned long long score = 0;
		//ʳ�ö�ƻ������
		unsigned long long eat_bad_apple = 0;
		//ʳ��ƻ������
		unsigned long long eat_apple = 0;
		//����Ƿ���
		bool is_live_ = true;
		//����ʹ�õ����巽��
		string body_block = SRC_BODY;
		//����ʹ�õ�ͷ������
		string head_block = SRC_HEAD;
		//ͷ������
		location* head_location = new location{ -1, -1, head_block };
		//��������
		std::vector<location*> bodys = std::vector<location*>();
		//β����
		location ll = { -1, -1, AIR };
		//�ƶ�����
		int move_direction = MOVE_RIGHT;
		//�ƶ����򣨻��棬Ϊ�����������ƶ�BUG��
		int move_direction_cache = move_direction;
		//��ݼ�
		key move_key = { 72, 80, 77, 75 };
	};

	inline bool operator== (const Player & p1, const Player & p2) { return &p1 == &p2; }

}

#endif Player_
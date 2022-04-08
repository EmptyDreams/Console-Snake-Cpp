#include "pch.h"
#include "Player.h"
#include <conio.h>

using minedreams::Player;

const string Player::SRC_BODY = "��";
const string Player::SRC_HEAD = "@";
string Player::AIR;

void Player::set_head_block(string & head_block)
{
	if (head_block.size() > 2)
		throw std::range_error("�ַ������Ȳ�Ӧ�ô���[2]");
	this->head_block = head_block;
}

void Player::set_body_block(string & body_block)
{
	if (body_block.size() > 2)
		throw std::range_error("�ַ������Ȳ�Ӧ�ô���[2]");
	this->body_block = body_block;
}

void Player::plus_length()
{
	++length;
	location* pl = new location{ ll.x, ll.y, body_block };
	bodys.push_back(pl);
}

void Player::minus_length()
{
	--length;
	delete bodys.back();
	bodys.erase(std::find(bodys.begin(), bodys.end(), bodys.back()));
}

void Player::plus_score(int amount)
{
	if (amount < 0 && score < -amount)
		die();
	else
		score += amount;
}

//�ƶ�
//������
//	x - x��ƫ����
//	y - y��ƫ����
bool Player::move_(const int x, const int y)
{
	//����1 - �洢��һ������Ҫ�ƶ�����λ��
	int cache[2] = { head_location->x, head_location->y };
	if (head_location->x + x < 0 || head_location->y + y < 0)
	{
		die();
		return false;
	}
	if (length == 1)
	{
		ll.x = head_location->x;
		ll.y = head_location->y;
	}
	else
	{
		ll.x = bodys.back()->x;
		ll.y = bodys.back()->y;
	}
	head_location->y += y;
	head_location->x += x;
	//����2 - �洢��һ�����������
	int cache2[2] = { -1,-1 };
	//��������
	if (length == 1) return true;
	int s = bodys.size();
	for (auto p : bodys)
	{
		if (p == nullptr) continue;
		//�ж�ͷ���Ƿ�������غ�
		//����غϼ�����ͷ����������ײ
		if (head_location->x == cache[0] && head_location->y == cache[1])
		{
			die();
			return false;
		}
		//��������
		cache2[0] = p->x;
		cache2[1] = p->y;
		p->x = cache[0];
		p->y = cache[1];
		cache[0] = cache2[0];
		cache[1] = cache2[1];
	}
	return true;
}

bool Player::move(const int direction)
{
	switch (direction)
	{
	case MOVE_UP:
		return move_(0, -1);
		break;
	case MOVE_DOWN:
		return move_(0, 1);
		break;
	case MOVE_RIGHT:
		return move_(1, 0);
		break;
	case MOVE_LEFT:
		return move_(-1, 0);
		break;
	default:
		return false;
	}
}

void Player::set_direction(int direction)
{
	if (direction == -move_direction)
		return;
	move_direction_cache = direction;
}

Player::~Player()
{
	//�ͷ�ͷ��
	delete head_location;
	//�ͷ�����
	for (location* pl : bodys)
		delete pl;
}
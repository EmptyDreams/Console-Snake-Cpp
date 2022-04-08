#include "pch.h"
#include "Player.h"
#include <conio.h>

using minedreams::Player;

const string Player::SRC_BODY = "■";
const string Player::SRC_HEAD = "@";
string Player::AIR;

void Player::set_head_block(string & head_block)
{
	if (head_block.size() > 2)
		throw std::range_error("字符串长度不应该大于[2]");
	this->head_block = head_block;
}

void Player::set_body_block(string & body_block)
{
	if (body_block.size() > 2)
		throw std::range_error("字符串长度不应该大于[2]");
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

//移动
//参数：
//	x - x轴偏移量
//	y - y轴偏移量
bool Player::move_(const int x, const int y)
{
	//缓存1 - 存储下一节身体要移动到的位置
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
	//缓存2 - 存储上一节身体的坐标
	int cache2[2] = { -1,-1 };
	//遍历身体
	if (length == 1) return true;
	int s = bodys.size();
	for (auto p : bodys)
	{
		if (p == nullptr) continue;
		//判断头部是否和身体重合
		//如果重合即代表头部与自身碰撞
		if (head_location->x == cache[0] && head_location->y == cache[1])
		{
			die();
			return false;
		}
		//更新数据
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
	//释放头部
	delete head_location;
	//释放身体
	for (location* pl : bodys)
		delete pl;
}
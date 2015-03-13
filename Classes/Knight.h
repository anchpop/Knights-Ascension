#pragma once

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class Knight : public cocos2d::Sprite
{
private:
public:

	Point position();
	Point tileCoordPosition();

	void setPlayerPosition(Point position);

};


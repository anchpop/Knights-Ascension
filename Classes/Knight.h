#pragma once

#include "cocos2d.h"
#include "TileUtils.h"
using namespace cocos2d;
using namespace std;

class Knight : public cocos2d::Sprite
{
private:
	TMXTiledMap *_tileMap;
	TMXLayer *_background;
	TMXLayer *_meta;
	TileMapTools _tmxdat;
public:
	
	void setKnightPosition(Point position, const std::function<void()>&, const std::function<void()>&);
	//void setup(String, TMXTiledMap*, TMXLayer*, TMXLayer*, int, int);

	Knight(const std::string&, TMXTiledMap*, TMXLayer*, TMXLayer*, TileMapTools);
	~Knight();
	static Knight* create(const std::string&, TMXTiledMap*, TMXLayer*, TMXLayer*, TileMapTools);
	//static Knight* create(const std::string&, const std::string&, TileMapTools);
	void initOptions();

	void doneWithMovement();
};


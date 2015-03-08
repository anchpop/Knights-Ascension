#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class KnightWorld : public cocos2d::Layer
{
private:
	TMXTiledMap *_tileMap;
	TMXLayer *_background;
	Sprite *_player;
	TMXLayer *_meta;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);


	void setViewPointCenter(Point position);

	Point tileCoordForPosition(Point position);

	void setPlayerPosition(Point position);

	void nana(Point position);


	// implement the "static create()" method manually
	CREATE_FUNC(KnightWorld);
};


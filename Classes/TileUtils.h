#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class TileMapTools
{
public:
	int tilestall;
	int tileswide;
	int tileheight;
	int tilewidth;

	cocos2d::Point tileCoordForPosition(const cocos2d::Point&);

	cocos2d::Point positionForTileCoord(const cocos2d::Point&);

	cocos2d::Point centerPositionForTileCoord(const cocos2d::Point&);

	cocos2d::Point roundedPosition(const cocos2d::Point&);

	cocos2d::Point roundedCenterPosition(const cocos2d::Point&);

	bool tileCoordInMapBounds(const cocos2d::Point&);

	TileMapTools();

	TileMapTools(int, int, int, int);

};
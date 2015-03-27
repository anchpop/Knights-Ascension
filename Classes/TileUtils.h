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

	cocos2d::Point tileCoordForPosition(cocos2d::Point);

	cocos2d::Point positionForTileCoord(cocos2d::Point);

	cocos2d::Point centerPositionForTileCoord(cocos2d::Point);

	TileMapTools();

	TileMapTools(int, int, int, int);

};
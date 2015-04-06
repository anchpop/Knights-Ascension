#pragma once

#include "cocos2d.h"
#include "Piece.h"
using namespace cocos2d;

class TileMapTools
{
public:
	int tilestall;
	int tileswide;
	int tileheight;
	int tilewidth;
	
	TMXTiledMap* map;
	TMXLayer *_background;
	//TMXLayer *_spawns;

	std::vector<Piece*> pieces;


	cocos2d::Point tileCoordForPosition(const cocos2d::Point&);

	cocos2d::Point positionForTileCoord(const cocos2d::Point&);

	cocos2d::Point centerPositionForTileCoord(const cocos2d::Point&);

	cocos2d::Point roundedPosition(const cocos2d::Point&);

	cocos2d::Point roundedCenterPosition(const cocos2d::Point&);

	bool tileCoordInMapBounds(const cocos2d::Point&);

	TileMapTools();

	TileMapTools(TMXTiledMap*, std::vector<Piece*> pieces);

	std::string checkSquareProperty(Vec2 square, const std::string &property, TMXLayer* layer);

};
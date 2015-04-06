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



	cocos2d::Point tileCoordForPosition(const cocos2d::Point&, bool yflip = true);

	cocos2d::Point positionForTileCoord(const cocos2d::Point&, bool yflip = true);

	cocos2d::Point centerPositionForTileCoord(const cocos2d::Point&, bool yflip = true);

	cocos2d::Point roundedPosition(const cocos2d::Point&);

	cocos2d::Point roundedCenterPosition(const cocos2d::Point&);

	bool tileCoordInMapBounds(const cocos2d::Point&);

	TileMapTools();

	TileMapTools(TMXTiledMap*);
	

	std::string checkSquareProperty(Vec2 square, const std::string &property, TMXLayer* layer);

	Piece* getPieceInSquare(const cocos2d::Point&, std::vector<Piece*>&);
};
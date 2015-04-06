#pragma once

#include "cocos2d.h"
#include "Piece.h"
using namespace cocos2d;

class TileMapTools
{
public:
    TileMapTools();

    TileMapTools(TMXTiledMap*);

    Point tileCoordForPosition(const Point&, bool yflip = true);

    Point positionForTileCoord(const Point&, bool yflip = true);

    Point centerPositionForTileCoord(const Point&, bool yflip = true);

    Point roundedPosition(const Point&);

    Point roundedCenterPosition(const Point&);

    bool tileCoordInMapBounds(const Point&);

    std::string checkSquareProperty(Vec2 square, const std::string &property, TMXLayer* layer);

    Piece* getPieceInSquare(const Point&, std::vector<Piece *> &);


    static const int tilestall = 16;
    static const int tileswide = 15;
    static const int tileheight = 64;
    static const int tilewidth  = 64;
	
    TMXTiledMap *_map;
    TMXLayer    *_background;
    //TMXLayer *_spawns;
};

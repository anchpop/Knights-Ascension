#pragma once

#include "Piece.h"
#include "TileUtils.h"
#include "SimpleAudioEngine.h" 
using namespace cocos2d;
using namespace std;

enum SquareType 
{
    BlockedSquare = 4, // These will have to change whenever the number of tiles changes
    HoleSquare    = 5, // These will have to change whenever the number of tiles changes
    EmptySquare   = 8  // These will have to change whenever the number of tiles changes
};                         


class Knight : public Piece
{
public:
    Knight(const std::string&, TileMapTools&);

    ~Knight();

    void setKnightPosition(Point position, const std::function<void()>&, const std::function<void()>&, vector<Piece*>&);

    //void setup(String, TMXTiledMap*, TMXLayer*, TMXLayer*, int, int);

    static Knight* create(const std::string&, TileMapTools&);

    //static Knight* create(const std::string&, const std::string&, TileMapTools);

    void initOptions();

    vector<Vec2> possibleSquaresToMoveOn(vector<Piece*>);

    bool canMoveToPoint(Vec2, vector<Piece*>);

private:
    void setSquare(SquareType squareType, Vec2 position) { _background->setTileGID(squareType, position); }
    vector<Vec2> passthroughsquares(Vec2);
    int getsign(int x) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }
    Vec2 getFirstLoc(Vec2, Vec2);

    TMXTiledMap *_tileMap;
    TMXLayer    *_background;
    TMXLayer    *_meta;
    TileMapTools _tmxdat;
};


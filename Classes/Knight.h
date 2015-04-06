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
    void setKnightPosition(Point position, const std::function<void()>&, const std::function<void()>&);
    //void setup(String, TMXTiledMap*, TMXLayer*, TMXLayer*, int, int);

    Knight(const std::string&, TileMapTools&);
    ~Knight();
    static Knight* create(const std::string&, TileMapTools&);
    //static Knight* create(const std::string&, const std::string&, TileMapTools);
    void initOptions();

    vector<Vec2> possibleSquaresToMoveOn();
    bool canMoveToPoint(Vec2);

private:
	void setSquare(SquareType squareType, Vec2 position) { _background->setTileGID(squareType, position); }

    TMXTiledMap *_tileMap;
    TMXLayer    *_background;
    TMXLayer    *_meta;
    TileMapTools _tmxdat;
    PieceType    _pieceType;
    PieceTeam    _team;
};


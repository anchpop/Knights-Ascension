#pragma once

#include "Piece.h"
#include "TileUtils.h"
#include "SimpleAudioEngine.h" 
#include "Knight.h" 
using namespace cocos2d;
using namespace std;

class Powerup : public Knight
{
public:
    Powerup(const std::string&, TileMapTools&);
    Powerup();

    ~Powerup();

    static Powerup* create(const std::string&, TileMapTools&);

    void initOptions();

    vector<Vec2> possibleSquaresToMoveOn(vector<Piece*> pieces);

    void setKnightPosition(Point position,
        const std::function<void()>& callWhenBeginMoving,
        const std::function<void()>& callWhenDoneMoving,
        vector<Piece*>& pieces);

    void take(vector<Piece*>&, Piece*);

protected:

    TMXTiledMap *_tileMap;
    TMXLayer    *_background;
    TMXLayer    *_meta;
    TileMapTools _tmxdat;
};


#pragma once

#include "Piece.h"
#include "TileUtils.h"
#include "SimpleAudioEngine.h" 
#include "Knight.h" 
using namespace cocos2d;
using namespace std;

class KnightK : public Knight
{
public:
    KnightK(const std::string&, TileMapTools&);
    KnightK();

    ~KnightK();

    static KnightK* create(const std::string&, TileMapTools&);

    void initOptions();

    vector<Vec2> possibleSquaresToMoveOn(vector<Piece*> pieces);

    void setKnightPosition(Point position,
        const std::function<void()>& callWhenBeginMoving,
        const std::function<void()>& callWhenDoneMoving,
        vector<Piece*>& pieces);

    void take(vector<Piece*>&);

protected:

    TMXTiledMap *_tileMap;
    TMXLayer    *_background;
    TMXLayer    *_meta;
    TileMapTools _tmxdat;
};


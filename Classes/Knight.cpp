#include "Knight.h"
#include <vector>

Knight::Knight(const string& FrameName, TileMapTools &tmxdat) : 
    _tmxdat(tmxdat)
{
    initWithFile(FrameName);
    initOptions();

    _tileMap    = _tmxdat._map;
    _background = _tmxdat._background;
    _pieceType  = TypeKnight;
}

Knight::~Knight() 
{
}

Knight* Knight::create(const string& FrameName, TileMapTools &tmxdat)
{
    Knight* sprite = new (std::nothrow) Knight(FrameName, tmxdat);
    sprite->autorelease();
    return sprite;
}


void Knight::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
}


void Knight::setKnightPosition(Point position,
                               const std::function<void()>& callWhenBeginMoving, 
                               const std::function<void()>& callWhenDoneMoving) 
{
    auto winSize = Director::getInstance()->getWinSize();
    Point tileCoord = _tmxdat.tileCoordForPosition(position);
    vector<Vec2> pPositions = possibleSquaresToMoveOn();

    if (_tmxdat.tileCoordInMapBounds(tileCoord))
    {
        if (std::find_if(pPositions.begin(), pPositions.end(), 
                         [tileCoord](Vec2 i) {return tileCoord == i; }) != pPositions.end())
        {
			
            callWhenBeginMoving();
            if (_tmxdat.checkSquareProperty(tileCoord, "Destroyable", _background) != "true"){
                runAction(Sequence::create(
                              EaseIn::create(MoveTo::create(0.6f, _tmxdat.roundedCenterPosition(position)), 2.5f),
                              CCCallFunc::create(callWhenDoneMoving),
                              nullptr));
            }
            else
            {
                runAction(Sequence::create(
                              EaseIn::create(MoveTo::create(0.2f, _tmxdat.roundedCenterPosition(position)), .3f),
                              CCCallFunc::create(callWhenDoneMoving),
                              CCCallFunc::create(
                                  [this, tileCoord]()
                                  {
                                      setSquare(EmptySquare, tileCoord); 
                                      CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
                                          "sound/explosion.wav");
                                  }),
                              nullptr));
            }
        }
    }
}

vector<Vec2> Knight::possibleSquaresToMoveOn()
{
    static const vector<Vec2> relativePossibleKnightSquaresToMoveOn = 
        { 
            Vec2( 1,  2), 
            Vec2( 2,  1),
            Vec2(-1,  2),
            Vec2(-2,  1),
            Vec2( 1, -2),
            Vec2( 2, -1),
            Vec2(-1, -2),
            Vec2(-2, -1)
        };

    static const vector<Vec2> relativePossibleBlockSquaresToMoveOn = 
        {
            Vec2(0, 1),
            Vec2(1, 0),
            Vec2(0, -1),
            Vec2(-1, 0)
        };

    Vec2 tileCoord = _tmxdat.tileCoordForPosition(getPosition());

    vector<Vec2> realPositions;

    for (std::size_t i = 0; i < relativePossibleKnightSquaresToMoveOn.size(); i++)
    {
        auto newPos = tileCoord + relativePossibleKnightSquaresToMoveOn[i];
        if (_tmxdat.checkSquareProperty(newPos, "Collideable", _background) != "true") 
        {  
            // if it's out of the map bounds it'll return "", so no problem there
            realPositions.push_back(newPos);
        }
    }

    for (std::size_t i = 0; i < relativePossibleBlockSquaresToMoveOn.size(); i++)
    {
        auto newPos = tileCoord + relativePossibleBlockSquaresToMoveOn[i];
        if (_tmxdat.checkSquareProperty(newPos, "Destroyable", _background) == "true")
            realPositions.push_back(newPos);
    }
	
    return realPositions;
}

bool Knight::canMoveToPoint(Vec2 spot)
{
    Vec2 tileCoord = _tmxdat.positionForTileCoord(spot);
    auto pPositions = Knight::possibleSquaresToMoveOn();
    return std::find_if(pPositions.begin(), pPositions.end(), 
                        [tileCoord](Vec2 i){return tileCoord == i; }) != pPositions.end();
}

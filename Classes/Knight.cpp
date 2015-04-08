#include "Knight.h"
#include <vector>

Knight::Knight() { }

Knight::Knight(const string& FrameName, TileMapTools &tmxdat) : 
    _tmxdat(tmxdat)
{
    initWithFile(FrameName);
    initOptions();

    _tileMap    = _tmxdat._map;
    _background = _tmxdat._background;
    pieceType = TypeKnight;
    ascended = false;
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
                               const std::function<void()>& callWhenDoneMoving,
                               vector<Piece*>& pieces)
{
    auto winSize = Director::getInstance()->getWinSize();
    Point tileCoord = _tmxdat.tileCoordForPosition(position);
    Point curPos = _tmxdat.tileCoordForPosition(getPosition());
    vector<Vec2> pPositions = possibleSquaresToMoveOn(pieces);

    if (_tmxdat.tileCoordInMapBounds(tileCoord))
    {
        if (std::find_if(pPositions.begin(), pPositions.end(), 
                         [tileCoord](Vec2 i) {return tileCoord == i; }) != pPositions.end())
        {
            
            callWhenBeginMoving();
            if (_tmxdat.checkSquareProperty(tileCoord, "Destroyable", _background) != "true"){   // If it's an empty square
                if (ascended)
                {
                    auto effect = (floor(CCRANDOM_0_1() * 3) == 0) ? "sound/crack.wav" : "sound/crack2.wav";
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effect);
                    setSquare(HoleSquare, curPos);
                }
                auto pieceToTake = _tmxdat.getPieceInSquare(tileCoord, pieces);
                //if (pieceToTake) pieceToTake->take(pieces);
                runAction(Sequence::create(
                    EaseIn::create(MoveTo::create(0.2f, _tmxdat.centerPositionForTileCoord(getFirstLoc(curPos, tileCoord))), 1.5f),
                    MoveTo::create(0.1f, _tmxdat.roundedCenterPosition(position)),
                    CCCallFunc::create([this, pieceToTake, &pieces](){if (pieceToTake) pieceToTake->take(pieces, this);}),
                    CCCallFunc::create(callWhenDoneMoving),
                    nullptr));
            }
            else
            {
                auto effect = (floor(CCRANDOM_0_1() * 3) == 0) ? "sound/break.wav" : "sound/break2.wav";
                runAction(Sequence::create(                                                     // If it's a blocked square
                              EaseIn::create(MoveTo::create(0.2f, _tmxdat.roundedCenterPosition(position)), .3f),
                              CCCallFunc::create(callWhenDoneMoving),
                              CCCallFunc::create(
                                [this, effect, tileCoord]()
                                  {
                                      setSquare(EmptySquare, tileCoord); 
                                      CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effect);
                                  }),
                              nullptr));
            }
        }
    }
}

vector<Vec2> Knight::possibleSquaresToMoveOn(vector<Piece*> pieces)
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
            bool squaresOpen = true;
            vector<Vec2> checksquares = passthroughsquares(relativePossibleKnightSquaresToMoveOn[i]);
            for (int i = 0; i < checksquares.size(); i++)
                if (_tmxdat.checkSquareProperty(checksquares[i] + tileCoord, "Collideable", _background) == "true" || 
                    _tmxdat.getPieceInSquare(checksquares[i] + tileCoord, pieces) != nullptr)
                    squaresOpen = false;

            if (squaresOpen)
            {
                auto p = _tmxdat.getPieceInSquare(newPos, pieces);
                // if it's out of the map bounds it'll return "", so no problem there
                if (p == nullptr || p->getTeam() != getTeam() || p->getTeam() == Neutral)
                    realPositions.push_back(newPos);
            }
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

bool Knight::canMoveToPoint(Vec2 spot, vector<Piece*> pieces)
{
    Vec2 tileCoord = _tmxdat.positionForTileCoord(spot);
    auto pPositions = Knight::possibleSquaresToMoveOn(pieces);
    return std::find_if(pPositions.begin(), pPositions.end(), 
                        [tileCoord](Vec2 i){return tileCoord == i; }) != pPositions.end();
}


vector<Vec2> Knight::passthroughsquares(Vec2 pos)
{
    if (pos == Vec2(1, 2) || pos == Vec2(-1, 2))
        return {Vec2(0, 1), Vec2(0,2)};
    else if (pos == Vec2(1, -2) || pos == Vec2(-1, -2))
        return{ Vec2(0, -1), Vec2(0, -2) };
    else if (pos == Vec2(2, 1) || pos == Vec2(2, -1))
        return{ Vec2(1, 0), Vec2(2, 0) };
    else if (pos == Vec2(-2, 1) || pos == Vec2(-2, -1))
        return{ Vec2(-1, 0), Vec2(-2, 0) };
    else return{};
}

Vec2 Knight::getFirstLoc(Vec2 start, Vec2 end)
{
    auto diff = end - start;
    if (diff == Vec2(1, 2) || diff == Vec2(-1, 2))
        return Vec2(start.x, start.y + 2);
    else if (diff == Vec2(1, -2) || diff == Vec2(-1, -2))
        return Vec2(start.x, start.y-2);
    else if (diff == Vec2(2, 1) || diff == Vec2(2, -1))
        return Vec2(start.x + 2, start.y);
    else //if (diff == Vec2(-2, 1) || diff == Vec2(-2, -1))
        return Vec2(start.x - 2, start.y);
}
#include "KnightK.h"
#include <vector>

KnightK::KnightK() { }

KnightK::KnightK(const string& FrameName, TileMapTools &tmxdat) : 
    _tmxdat(tmxdat)
{
    initWithFile(FrameName);
    initOptions();

    _tileMap    = _tmxdat._map;
    _background = _tmxdat._background;
    pieceType  = TypeKing;
}

KnightK::~KnightK() 
{
}

KnightK* KnightK::create(const string& FrameName, TileMapTools &tmxdat)
{
    KnightK* sprite = new (std::nothrow) KnightK(FrameName, tmxdat);
    sprite->autorelease();
    return sprite;
}


void KnightK::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
}


void KnightK::setKnightPosition(Point position,
                               const std::function<void()>& callWhenBeginMoving, 
                               const std::function<void()>& callWhenDoneMoving,
                               vector<Piece*>& pieces)
{
    Point tileCoord = _tmxdat.tileCoordForPosition(position);
    vector<Vec2> pPositions = possibleSquaresToMoveOn(pieces);

    if (_tmxdat.tileCoordInMapBounds(tileCoord))
    {
        if (std::find_if(pPositions.begin(), pPositions.end(), 
                         [tileCoord](Vec2 i) {return tileCoord == i; }) != pPositions.end())
        {
			
            callWhenBeginMoving();
            auto pieceToTake = _tmxdat.getPieceInSquare(tileCoord, pieces);
            runAction(Sequence::create(
                EaseInOut::create(MoveTo::create(2.5f, _tmxdat.roundedCenterPosition(position)), 1.5f),
                CCCallFunc::create([pieceToTake, &pieces](){if (pieceToTake) pieceToTake->take(pieces);}),
                CCCallFunc::create(callWhenDoneMoving),
                nullptr));
        }
    }
}

vector<Vec2> KnightK::possibleSquaresToMoveOn(vector<Piece*> pieces)
{
    static const vector<Vec2> relativePossibleSquaresToMoveOn = 
        {
            Vec2(0, 1),
            Vec2(1, 0),
            Vec2(0, -1),
            Vec2(-1, 0)
        };

    Vec2 tileCoord = _tmxdat.tileCoordForPosition(getPosition());

    vector<Vec2> realPositions;

    for (std::size_t i = 0; i < relativePossibleSquaresToMoveOn.size(); i++)
    {
        auto newPos = tileCoord + relativePossibleSquaresToMoveOn[i];
        if (_tmxdat.checkSquareProperty(newPos, "Collideable", _background) != "true")
        {
            auto p = _tmxdat.getPieceInSquare(newPos, pieces);
            // if it's out of the map bounds it'll return "", so no problem there
            if (p == nullptr || p->getTeam() != getTeam() || p->getTeam() == Neutral)
                realPositions.push_back(newPos);
        }
    }
	
    return realPositions;
}

void KnightK::take(std::vector<Piece*>& pieces)
{

    CCLOG("The King Has Been Taken!");
    EventCustom event("king taken");
    _eventDispatcher->dispatchEvent(&event);

    /*for (int i = 0; i < pieces.size(); i++)
    {
        if (pieces[i] == this)
        {
            pieces.erase(pieces.begin() + i);
            this->removeFromParentAndCleanup(true);
            break;
        }
    }*/
    
}
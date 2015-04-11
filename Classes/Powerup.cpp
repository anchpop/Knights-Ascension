#include "Powerup.h"
#include <vector>

Powerup::Powerup() { }

Powerup::Powerup(const string& FrameName, TileMapTools &tmxdat) : 
    _tmxdat(tmxdat)
{
    initWithFile(FrameName);
    initOptions();

    _tileMap    = _tmxdat._map;
    _background = _tmxdat._background;
    pieceType = TypePowerup;
    team = Neutral;
    ascended = false;
}

Powerup::~Powerup() 
{
}

Powerup* Powerup::create(const string& FrameName, TileMapTools &tmxdat)
{
    Powerup* sprite = new (std::nothrow) Powerup(FrameName, tmxdat);
    sprite->autorelease();
    return sprite;
}


void Powerup::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
}


void Powerup::setKnightPosition(Point position,
                               const std::function<void()>& callWhenBeginMoving, 
                               const std::function<void()>& callWhenDoneMoving,
                               vector<Piece*>& pieces)
{
   
}

vector<Vec2> Powerup::possibleSquaresToMoveOn(vector<Piece*> pieces)
{
    return { };
}

void Powerup::take(std::vector<Piece*>& pieces, Piece* takenby)
{
    if (takenby->getPieceType() == TypeKnight || takenby->getPieceType() == TypeKing)
        dynamic_cast<Knight *>(takenby)->ascend();
    for (int i = 0; i < pieces.size(); i++)
    {
        if (pieces[i] == this)
        {
            pieces.erase(pieces.begin() + i);
            this->removeFromParentAndCleanup(true);
            break;
        }
    }
    
}
#include "Piece.h"

using namespace cocos2d;

Piece::Piece() {}

Piece::~Piece() {}

Piece* Piece::create()
{
	Piece* pSprite = new Piece();

	if (pSprite->initWithSpriteFrameName("Piece.png"))
	{
		pSprite->autorelease();

		pSprite->initOptions();

        

		return pSprite;
	}

    

	CC_SAFE_DELETE(pSprite);
	return NULL;
}

void Piece::initOptions()
{
    wiggleingHasStopped = true;
    shouldWiggle = false;
}

void Piece::setTeam(PieceTeam _team)
{
	team = _team;
}

PieceTeam Piece::getTeam()
{
	return team;
}

PieceType Piece::getPieceType()
{
	return pieceType;
}

void Piece::kill(std::vector<Piece*>& pieces, Piece* takenby)
{
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

void Piece::take(std::vector<Piece*>& pieces, Piece* takenby)
{
    auto rect = VisibleRect::getVisibleRect().size;
    auto sign = floor(CCRANDOM_0_1() * 2) == 0 ? 1 : -1;
    runAction(Sequence::create(
        JumpBy::create(0.6f, Vec2(sign * rect.width / 3, -rect.height), 2 * rect.height / 3, 1),
        CCCallFunc::create([this, &pieces, &takenby](){kill(pieces, takenby); }),
        nullptr));
    runAction(RotateBy::create(.59f, 360 * 2));
    runAction(FadeOut::create(.59f));
}

void Piece::restartWiggle()
{
    if (shouldWiggle)
    {
        runAction(Sequence::create(
            EaseOut::create(RotateBy::create(.1f, 5.0f), 0.6f),
            EaseInOut::create(RotateBy::create(.2f, -10.0f), 2.0f),
            EaseIn::create(RotateBy::create(.1f, 5.0f), 0.6f),
            CCCallFunc::create([this](){ if (!shouldWiggle) wiggleingHasStopped = true; }),
            CCCallFunc::create([this](){restartWiggle(); }),
            nullptr
            ));
    }
}

void Piece::startWiggle()
{
    if (shouldWiggle && wiggleingHasStopped)
    {
        runAction(Sequence::create(
            //DelayTime::create(CCRANDOM_0_1()),  // put any delay you want here
            CCCallFunc::create([this](){ wiggleingHasStopped = false; }),
            CCCallFunc::create([this](){ restartWiggle(); }),
            nullptr
            ));
    }
}
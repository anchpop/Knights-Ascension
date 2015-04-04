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
	// do things here like setTag(), setPosition(), any custom logic.
}
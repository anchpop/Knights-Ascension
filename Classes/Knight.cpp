#include "Knight.h"

using namespace cocos2d;

using namespace std;

Knight::Knight(const string& FrameName, TMXTiledMap *tileMap, TMXLayer *background, TileMapTools tmxdat)
{
	initWithFile(FrameName);
	

	initOptions();

	_tileMap = tileMap;
	_background = background; // this is probably not neccesary to be passed in if we replace it with _background = _tileMap->layerNamed("mainboard");
	_tmxdat = tmxdat;

	//return sprite;
}

Knight::~Knight() {}



Knight* Knight::create(const string& FrameName, TMXTiledMap *tileMap, TMXLayer *background, TileMapTools tmxdat)
{
	Knight* sprite = new (std::nothrow) Knight(FrameName, tileMap, background, tmxdat);
	sprite->autorelease();
	return sprite;
}


void Knight::initOptions()
{
	// do things here like setTag(), setPosition(), any custom logic.
}


void Knight::setKnightPosition(Point position, const std::function<void()>& callWhenBeginMoving, const std::function<void()>& callWhenDoneMoving) {
	auto winSize = Director::getInstance()->getWinSize();
	Point tileCoord = _tmxdat.tileCoordForPosition(position);
	if (!((tileCoord.x < 0) || (tileCoord.x > _tmxdat.tileswide) || (tileCoord.y < 0) || (tileCoord.y > _tmxdat.tilestall)))
	{
		int tileGid = _background->tileGIDAt(tileCoord);
		if (tileGid) {
			auto properties = _tileMap->propertiesForGID(tileGid).asValueMap();
			if (!properties.empty()) {
				auto collision = properties["Collideable"].asString();
				if (collision == "true") {
					return;
				}
			}
		}

		callWhenBeginMoving();
		runAction(Sequence::create(
			EaseIn::create(MoveTo::create(0.6f, _tmxdat.roundedCenterPosition(position)), 2.5f),
			CCCallFunc::create(
				callWhenDoneMoving),
			nullptr));
	}
}

#include "Knight.h"

using namespace cocos2d;

using namespace std;

Knight::Knight(const string& FrameName, TMXTiledMap *tileMap, TMXLayer *background, TMXLayer *meta, TileMapTools tmxdat)
{
	initWithFile(FrameName);
	

	initOptions();

	_tileMap = tileMap;
	_background = background;
	_meta = meta;
	_tmxdat = tmxdat;

	//return sprite;
}

Knight::~Knight() {}



Knight* Knight::create(const string& FrameName, TMXTiledMap *tileMap, TMXLayer *background, TMXLayer *meta, TileMapTools tmxdat)
{
	Knight* sprite = new (std::nothrow) Knight(FrameName, tileMap, background, meta, tmxdat);
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
		//_meta->setLayerSize(Size(tmxdat.tilestall, tmxdat.tileswide));
		int tileGid = _meta->tileGIDAt(tileCoord);
		if (tileGid) {
			auto properties = _tileMap->propertiesForGID(tileGid).asValueMap();
			if (!properties.empty()) {
				auto collision = properties["Collideable"].asString();
				if (collision == "true") {
					return;
				}
			}
		}
		//this->setPosition(_tmxdat.roundedCenterPosition(position));

		callWhenBeginMoving();
		runAction(Sequence::create(
			EaseIn::create(MoveTo::create(0.8f, _tmxdat.roundedCenterPosition(position)), 2.5f),
			CCCallFunc::create(
				callWhenDoneMoving),
			nullptr));
	}
}

void Knight::doneWithMovement() {
	// it reaches here
}
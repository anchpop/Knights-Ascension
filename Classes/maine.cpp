#include "maine.h"

#include "TileUtils.h"

using namespace cocos2d;

using namespace std;

Scene* KnightWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = KnightWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool KnightWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}


	// create a TMX map
	_tileMap = TMXTiledMap::create("map.tmx"); // note to self, consider using "new" here
	addChild(_tileMap, 0);
	_background = _tileMap->layerNamed("mainboard");

	// all tiles are aliased by default, let's set them anti-aliased
	for (const auto& child : _tileMap->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}

	auto objectGroup = _tileMap->objectGroupNamed("objects");

	if (objectGroup == NULL){
		CCLOG("tile map has no 'objects' object layer");
		return false;
	}
	CCLOG("tile map objects detected");

	_meta = _tileMap->layerNamed("Meta");
	_meta->setVisible(false);

	ValueMap spawnPoint = objectGroup->objectNamed("Team A spawn");

	int x; istringstream( ((spawnPoint.at("x")).getDescription()) ) >> x;
	int y; istringstream( ((spawnPoint.at("y")).getDescription()) ) >> y;

	_player = Sprite::create("imgs/sprite1.png");
	_player->setPosition(ccp(x, y));

	this->addChild(_player);
	this->setViewPointCenter(_player->getPosition());

	setTouchEnabled(true);

	this->setScale(0.8f); // Shrinkify everything!

	auto listener1 = EventListenerTouchOneByOne::create();

	// trigger when you push down
	listener1->onTouchBegan = [this](Touch* touch, Event* event){
		auto target = static_cast<Layer*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		//setViewPointCenter(locationInNode);
		setPlayerPosition(locationInNode);
		return true; // if you are consuming it
	};

	// trigger when moving touch
	/*listener1->onTouchMoved = [this](Touch* touch, Event* event){
		auto target = static_cast<Layer*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		//setViewPointCenter(locationInNode);
		setPlayerPosition(locationInNode);
	};*/ //Commenting this out because I don't like it. You can't stop me.

	// trigger when you let up
	listener1->onTouchEnded = [this](Touch* touch, Event* event){
		Point touchLocation = touch->getLocationInView();
		touchLocation = Director::getInstance()->convertToGL(touchLocation);
		touchLocation = this->convertToNodeSpace(touchLocation);

		Point playerPos = _player->getPosition();
		Point diff = ccpSub(touchLocation, playerPos);

		/*if (abs(diff.x) > abs(diff.y)) {
			if (diff.x > 0) {
				playerPos.x += 32;
			}
			else {
				playerPos.x -= 32;
			}
		}
		else {
			if (diff.y > 0) {
				playerPos.y += 32;
			}
			else {
				playerPos.y -= 32;
			}
		}*/

		// safety check on the bounds of the map
		/*if (playerPos.x <= (32 * 32) &&
			playerPos.y <= (32 * 32) &&
			playerPos.y >= 0 &&
			playerPos.x >= 0)
		{
			this->setPlayerPosition(playerPos);
		}*/
		/*// safety check on the bounds of the map
		if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
			playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
			playerPos.y >= 0 &&
			playerPos.x >= 0 )
		{
		    this->setPlayerPosition(playerPos);
		}*/

		setViewPointCenter(_player->getPosition());

	};

	// Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

	
	return true;
}



void KnightWorld::setViewPointCenter(Point position) {
	Point tileCoord = tileCoordForPosition(position);
	if (!((tileCoord.x < 0) || (tileCoord.x > tileswide) || (tileCoord.y < 0) || (tileCoord.y > tileswide)))
	{
		auto winSize = Director::getInstance()->getWinSize();

		/*int x = MAX(position.x, winSize.width / 2);
		int y = MAX(position.y, winSize.height / 2);

		x = MIN(x, (tileswide * tilesize) - winSize.width / 2); //_tileMap->getMapSize().width * this->_tileMap->getTileSize().width wasn't working :(
		y = MIN(y, (tileswide * tilesize) - winSize.height / 2);
		Point actualPosition = ccp(x, y);

		Point centerOfView = ccp(winSize.width / 2, winSize.height / 2);
		Point viewPoint = ccpSub(centerOfView, actualPosition);
		this->setPosition(viewPoint);


		//auto z = this->_tileMap->getTileSize();
		//z.width; // error is here*/

		this->setPosition((ccpSub(ccp(winSize.width / 2, winSize.height / 2), position)));
	}
}




void KnightWorld::setPlayerPosition(Point position) {
	auto winSize = Director::getInstance()->getWinSize();
	Point tileCoord = tileCoordForPosition(position);
	if (!((tileCoord.x < 0) || (tileCoord.x > tileswide) || (tileCoord.y < 0) || (tileCoord.y > tileswide)))
	{
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
		_player->setPosition(centerPositionForTileCoord(tileCoord));
	}
}
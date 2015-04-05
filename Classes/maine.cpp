#include "maine.h"
#include "TileUtils.h"
#include "VisibleRect.h"

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
	//_tileMap = new CCTMXTiledMap();
	//_tileMap->initWithTMXFile("TileMap.tmx");
	addChild(_tileMap, 0);
	_background = _tileMap->layerNamed("mainboard");

	tmxdat = TileMapTools(_tileMap, pieces);

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

	boardRPM = 1.0f / 60.0f;

	ValueMap spawnPoint = objectGroup->objectNamed("Team A spawn");

	int x; istringstream( ((spawnPoint.at("x")).getDescription()) ) >> x;
	int y; istringstream( ((spawnPoint.at("y")).getDescription()) ) >> y;

	
	//_player = Knight::create("imgs/sprite1.png", _tileMap, _background, _meta, tmxdat);
	//_player = Knight::create("imgs/sprite1.png", _tileMap, _background, _meta, tmxdat);
	//_player->setPosition(tmxdat.roundedCenterPosition(Vec2(x, y)));

	pieces.push_back(Knight::create("imgs/sprite1.png", _tileMap, tmxdat));
	pieces[0]->setPosition(tmxdat.roundedCenterPosition(Vec2(x, y)));
	pieces[0]->setTeam(TeamBlue);
	pieces.push_back(Knight::create("imgs/sprite2.png", _tileMap, tmxdat));
	pieces[1]->setPosition(tmxdat.centerPositionForTileCoord(Vec2(4, 4)));
	pieces[1]->setTeam(TeamRed);
	currentTeamTurn = TeamBlue;
	movesElapsed = 0;
	movesPerTurn = 3;

	activePiece = dynamic_cast<Knight *>(pieces[0]);
	spriteIsMoving = false;
	screenIsMoving = false;

	for (std::size_t i = 0; i < pieces.size(); i++)
	{
		this->addChild(pieces[i]);
	}
	this->setViewPointCenter(pieces[0]->getPosition());

	setTouchEnabled(true);

	
	this->setScale(0.8f); // Shrinkify everything!
	runAction(RepeatForever::create(RotateBy::create(60.0f / boardRPM, 360.0f)));
	//this->setRotation(45.0f); // Spinify everything!

	auto listener1 = EventListenerTouchOneByOne::create();

	
	// trigger when you push down
	listener1->onTouchBegan = [&](Touch* touch, Event* event){
		if (!spriteIsMoving && !screenIsMoving)
		{
			auto target = static_cast<Layer*>(event->getCurrentTarget());
			Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
			//setViewPointCenter(locationInNode);
			if (activePiece == nullptr)
			{
				for (std::size_t i = 0; i < pieces.size(); i++)
				{
					if (pieces[i]->boundingBox().containsPoint(locationInNode) && pieces[i]->getTeam() == currentTeamTurn)
					{
						activePiece = dynamic_cast<Knight *>(pieces[i]);
						break;
					}
				}
			}
			else
			{
				if (!activePiece->boundingBox().containsPoint(locationInNode))
				{
					
					activePiece->setKnightPosition(locationInNode, 
						[this, locationInNode](){ spriteIsMoving = true;                                                                       //When the sprite begins moving, set spriteIsMoving to true
							moveViewPointCenter(locationInNode,	[this](){screenIsMoving = true; }, [this](){screenIsMoving = false; }); },     //Also when the sprite begins moving, begin moving the screen opposite the sprite (to keep the sprite centered) and when the screen begins and ends moving, change screenIsMoving
						[this, locationInNode](){ activePiece = nullptr; spriteIsMoving = false; });                                           //When the sprite ends moving deselet it and mark it as not moving
					movesElapsed++;
					if (movesElapsed >= movesPerTurn)
					{
						currentTeamTurn = (currentTeamTurn == TeamRed) ? TeamBlue : TeamRed;
						movesElapsed = 0;
					}
				}
			}
		}

		return true; // if you are consuming it

	};

	// trigger when you let up
	listener1->onTouchEnded = [&](Touch* touch, Event* event){	
	};

	// Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

	
	return true;
}


Point KnightWorld::getPointToCenterOn(Point position) {
	Point tileCoord = tmxdat.tileCoordForPosition(position);
	if (!((tileCoord.x < 0) || (tileCoord.x > tmxdat.tileswide) || (tileCoord.y < 0) || (tileCoord.y > tmxdat.tilestall)))
	{
		auto layerpos = (convertToWorldSpace(VisibleRect::center()) - convertToWorldSpace(position));
		return layerpos;
	}
	return nullptr;
}


void KnightWorld::setViewPointCenter(Point position) {
	this->setPosition(getPointToCenterOn(position));
}

void KnightWorld::moveViewPointCenter(Point position, const std::function<void()>& callWhenDoneMoving, float time, float easing) {

	runAction(Sequence::create(
		EaseIn::create(MoveTo::create(time, getPointToCenterOn(position)), easing),
		CCCallFunc::create(
		callWhenDoneMoving),
		nullptr));
}

void KnightWorld::moveViewPointCenter(Point position, const std::function<void()>& callWhenBeginMoving, const std::function<void()>& callWhenDoneMoving, float time, float easing) {
	callWhenBeginMoving();
	moveViewPointCenter(position, callWhenDoneMoving);
}


#include "maine.h"
#include "TileUtils.h"
#include "VisibleRect.h"
#include <sstream>

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

    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
    //	"sound/sanctuary.mp3", true);                                            //Play background music


    // create a TMX map
    _tileMap = TMXTiledMap::create("map.tmx"); // note to self, consider using "new" here
    addChild(_tileMap, 0);
    _background = _tileMap->layerNamed("mainboard");


    // all tiles are aliased by default, let's set them anti-aliased
    for (const auto& child : _tileMap->getChildren())
    {
        static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
    }

    boardRPM = 1.0f / 60.0f;

    tmxdat = TileMapTools(_tileMap);

    _spawn = _tileMap->layerNamed("Meta");
    _spawn->setVisible(false);
    for (int x = 0; x < tmxdat.tileswide; ++x)
    {
        for (int y = 0; y < tmxdat.tilestall; ++y)
        {
            if (tmxdat.checkSquareProperty(Vec2(x, y), "Piece type", _spawn) == "RedNormal")
            {
                auto curr = Knight::create("imgs/sprite2.png", tmxdat);
                pieces.push_back(curr);
                curr->setPosition(tmxdat.centerPositionForTileCoord(Vec2(x, y)));
                curr->setTeam(TeamRed);
            }
            if (tmxdat.checkSquareProperty(Vec2(x, y), "Piece type", _spawn) == "RedKing")
            {
                auto curr = KnightK::create("imgs/sprite2king.png", tmxdat);
                pieces.push_back(curr);
                curr->setPosition(tmxdat.centerPositionForTileCoord(Vec2(x, y)));
                curr->setTeam(TeamRed);
            }
            if (tmxdat.checkSquareProperty(Vec2(x, y), "Piece type", _spawn) == "BlueNormal")
            {
                auto curr = Knight::create("imgs/sprite1.png", tmxdat);
                pieces.push_back(curr);
                curr->setPosition(tmxdat.centerPositionForTileCoord(Vec2(x, y)));
                curr->setTeam(TeamBlue);
            }
            if (tmxdat.checkSquareProperty(Vec2(x, y), "Piece type", _spawn) == "BlueKing")
            {
                auto curr = KnightK::create("imgs/sprite1king.png", tmxdat);
                pieces.push_back(curr);
                curr->setPosition(tmxdat.centerPositionForTileCoord(Vec2(x, y)));
                curr->setTeam(TeamBlue);
            }
        }
    }
	
    currentTeamTurn = pieces[0]->getTeam();
    movesElapsed = 0;
    movesPerTurn = 3;

    activePiece = nullptr;
    spriteIsMoving = false;
    screenIsMoving = false;

    for (std::size_t i = 0; i < pieces.size(); i++)
    {
        this->addChild(pieces[i], 20);
    }
    this->setViewPointCenter(pieces[0]->getPosition());

    setTouchEnabled(true);

	
    this->setScale(0.8f); // Shrinkify everything!
    //runAction(RepeatForever::create(RotateBy::create(60.0f / boardRPM, 360.0f)));
    //this->setRotation(45.0f); // Spinify everything!


    TTFConfig ttfConfig("fonts/pixel.ttf", 60, GlyphCollection::NEHE);                                              // I have no idea how any of this works
    ttfConfig.fontFilePath = "fonts/pixel.ttf";                                                                     // I have no idea how any of this works
    teamLabel = Label::createWithTTF(ttfConfig, "fonts/pixel.ttf", TextHAlignment::CENTER, 0);                      // I have no idea how any of this works
    //teamLabel = Label::createWithSystemFont("Red Team turn " + to_string(movesPerTurn - movesElapsed) + "", , 48);
    teamLabel->setString("Red Team turn (" + to_string(movesPerTurn - movesElapsed) + ")");
    teamLabel->setPosition(tmxdat.centerPositionForTileCoord(Vec2(tmxdat.tileswide / 2.0f, -1.0f)));
    teamLabel->enableShadow(Color4B(0, 0, 0, 150), Size(3,3), 0);
    this->addChild(teamLabel, 1);
    teamLabel->setColor(ccc3(255, 0, 0));
	

    // Init gesture recognizer
    initGestureRecognizer();
	
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
                    auto onmovingstart = [this, locationInNode]()
                    {
                        spriteIsMoving = true;
                        movesElapsed++;  //When the sprite begins moving, set spriteIsMoving to true
                        moveViewPointCenter(locationInNode,
                            [this](){screenIsMoving = true;  },
                            [this](){screenIsMoving = false; });
                    };

                    auto onmovingend = [this, locationInNode]()
                    {
                        activePiece = nullptr;
                        spriteIsMoving = false;
                    };

                    activePiece->setKnightPosition(locationInNode,
                        onmovingstart,
                        // Also when the sprite begins moving, begin moving the screen 
                        // opposite the sprite (to keep the sprite centered) and when 
                        // the screen begins and ends moving, change screenIsMoving
                        onmovingend,  //When the sprite ends moving deselet it and mark it as not moving
                        pieces
                        );

                    if (movesElapsed >= movesPerTurn)
                    {
                        currentTeamTurn = (currentTeamTurn == TeamRed) ? TeamBlue : TeamRed;
                        movesElapsed = 0;
                        auto paren = " (" + to_string(movesPerTurn - movesElapsed) + ")";
                        teamLabel->setString((currentTeamTurn == TeamRed) ? "Red Team turn" + paren : "Blue Team turn" + paren);
                        teamLabel->setColor((currentTeamTurn == TeamRed) ? ccc3(255, 0, 0) : ccc3(0, 0, 255));
                        teamLabel->setScale(1.4f);
                        teamLabel->runAction(EaseOut::create(ScaleTo::create(.3f, 1.0f), 0.3f));
                    }
                    else
                    {
                        auto paren = " (" + to_string(movesPerTurn - movesElapsed) + ")";
                        teamLabel->setString((currentTeamTurn == TeamRed) ? "Red Team turn" + paren : "Blue Team turn" + paren);
                    }
                }
                else
                    activePiece = nullptr;
            }
        }

        return false; // if you are consuming it

    };

    // Add listener
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

    return true;
}


Point KnightWorld::getPointToCenterOn(Point position) 
{
    Point tileCoord = tmxdat.tileCoordForPosition(position);
    if (!((tileCoord.x < 0) || (tileCoord.x > tmxdat.tileswide) || 
          (tileCoord.y < 0) || (tileCoord.y > tmxdat.tilestall)))
    {
        return (convertToWorldSpace(VisibleRect::center()) - convertToWorldSpace(position));
    }
    return nullptr;
}


void KnightWorld::setViewPointCenter(Point position)
{
	this->setPosition(getPointToCenterOn(position));
}

void KnightWorld::moveViewPointCenter(Point position, 
                                      const std::function<void()>& callWhenDoneMoving, 
                                      float time, 
                                      float easing) 
{

    runAction(Sequence::create(
                  EaseIn::create(MoveTo::create(time, getPointToCenterOn(position)), easing),
                  CCCallFunc::create(
                      callWhenDoneMoving),
                  nullptr));
}

void KnightWorld::moveViewPointCenter(Point position, 
                                      const std::function<void()>& callWhenBeginMoving,
                                      const std::function<void()>& callWhenDoneMoving,
                                      float time,
                                      float easing) 
{
    callWhenBeginMoving();
    moveViewPointCenter(position, callWhenDoneMoving);
}



void KnightWorld::initGestureRecognizer()
{
    // Init gesture recognizer
    _gestureRecognizer = new TSimpleGestureRecognizer();
    _gestureRecognizer->init();
    _gestureRecognizer->setGestureHandler(this);

    // Enable all gesture kinds
    _gestureRecognizer->setTapEnabled(false);
    _gestureRecognizer->setDoubleTapEnabled(false);
    _gestureRecognizer->setLongPressEnabled(false);
    _gestureRecognizer->setPanEnabled(true);
    _gestureRecognizer->setPinchEnabled(true);
    _gestureRecognizer->setRotationEnabled(false);
    _gestureRecognizer->setSwipeEnabled(false);

    // Taps will be fired immediately without waiting for double tap
    _gestureRecognizer->setTapRequiresDoubleTapRecognitionToFail(false);

    // Other config
    // _gestureRecognizer->setTapThreshold(1.0f);
    // _gestureRecognizer->setLongPressThreshold(1.0f);
    // _gestureRecognizer->setDoubleTapInterval(0.3f);
    // _gestureRecognizer->setPinchFingersDistanceChangeTolerance(0.1f);
    // _gestureRecognizer->setRotationFingersDistanceChangeTolerance(0.5f);
    // _gestureRecognizer->setSwipeThreshold(0.3f);

    //
    // IMPORTANT:
    // For multiple touch gestures on iOS (pinch, rotation), always remember tu put
    // the below line of code right after creating the CCEAGLView in AppController.mm
    // [eaglView setMultipleTouchEnabled:YES];
    // For Android, there no need to do this.
    //

    // Create touch listener and register it with cocos2d to receive touch events
    EventListenerTouchOneByOne* touchDelegate = EventListenerTouchOneByOne::create();
    touchDelegate->onTouchBegan = std::bind(&KnightWorld::TouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchMoved = std::bind(&KnightWorld::TouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchCancelled = std::bind(&KnightWorld::TouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchEnded = std::bind(&KnightWorld::TouchEnded, this, std::placeholders::_1, std::placeholders::_2);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchDelegate, 100);
}


bool KnightWorld::onGesturePan(TGesturePan* gesture)
{
    auto coord = convertToNodeSpace(gesture->getLocation());
    if (tmxdat.getPieceInSquare(tmxdat.tileCoordForPosition(coord), pieces) == nullptr && !spriteIsMoving && !screenIsMoving)
    {
        static int lastPanId = -1;
        static bool panInMapBoundry = false;
        if (gesture->getID() != lastPanId)
        {
            lastPanId = gesture->getID(); //This currently does nothing but could be used to not have a constant bombardment of pans
            panInMapBoundry = tmxdat.tileCoordInMapBounds(tmxdat.tileCoordForPosition(convertToNodeSpace(gesture->getLocation()))); // needs work
        }
        if (panInMapBoundry)
        {
            this->setPosition(this->getPosition() + gesture->getTranslation());
        }
    }
    return false;
}

bool KnightWorld::onGesturePinch(TGesturePinch* gesture)
{
    /*static int lastPinchId = -1;
      static bool pinchInsideNode = false;
      static float originalScale;

      // A new pinch
      if (gesture->getID() != lastPinchId)
      {
      lastPinchId = gesture->getID();
      pinchInsideNode = NodeContainsPoint(_sprite, gesture->getLocation());
      originalScale = _sprite->getScale();
      }

      if (pinchInsideNode)
      {
      _sprite->setScale(originalScale * gesture->getScale());
      }
    */
    return false;
}


bool KnightWorld::TouchBegan(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    _gestureRecognizer->onTouchBegan(touch, event);
    return true;
}

void KnightWorld::TouchMoved(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    _gestureRecognizer->onTouchMoved(touch, event);
}

void KnightWorld::TouchEnded(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    _gestureRecognizer->onTouchEnded(touch, event);
}

void KnightWorld::TouchCancelled(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    _gestureRecognizer->onTouchCancelled(touch, event);
}


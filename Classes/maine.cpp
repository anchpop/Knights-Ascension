#include "maine.h"
#include "TileUtils.h"
#include "shake.h"
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
    scene->addChild(layer, 1);

    auto backgroundlayer = Layer::create();
    auto foregroundlayer = Layer::create();
    auto bg = Sprite::create("imgs/backdrop.jpg");
    scene->addChild(backgroundlayer, 0); 
    scene->addChild(foregroundlayer, 20);
    backgroundlayer->addChild(bg, 0);
    auto button = ui::Button::create("imgs/reset.png",
        "imgs/resetpushed.png");
    button->setAnchorPoint(Vec2(1.2f, 1.2f));
    foregroundlayer->addChild(button);
    button->addTouchEventListener([layer](Ref* sender, ui::Widget::TouchEventType type){
        layer->resetGame();
    });
    button->setScale(0.8f);
    button->setPosition(VisibleRect::rightTop());
    bg->setAnchorPoint(Vec2(.5f, .5f));
    bg->setPosition(VisibleRect::center());
    bg->setScale(1.0f);

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
    addChild(_tileMap, 1);
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
	
    //currentTeamTurn = TeamRed;
    setCurTeam(TeamBlue);
    movesElapsed = 0;
    totalTurnsPassed = 0;
    movesPerTurn = 4;

    setActivePiece(nullptr);
    spriteIsMoving = false;
    screenIsMoving = false;

    for (std::size_t i = 0; i < pieces.size(); i++)
    {
        this->addChild(pieces[i], 20);
    }

    setTouchEnabled(true);

    //this->setScale(2.0f); // Shrinkify everything!
    //runAction(RepeatForever::create(RotateBy::create(60.0f / boardRPM, 360.0f)));
    //this->setRotation(45.0f); // Spinify everything!

    auto pos = tmxdat.positionForTileCoord(Vec2(7.5, 8));
    this->setViewPointCenter(pos);

    
    initiateTeamText();

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
                        if (pieces[i]->getPieceType() != TypeKing || movesElapsed == 0)
                            setActivePiece(dynamic_cast<Knight *>(pieces[i]));
                        break;
                    }
                }
            }
            else
            {
                if (!activePiece->boundingBox().containsPoint(locationInNode))
                {
                    auto tileCoord = tmxdat.tileCoordForPosition(locationInNode);
                    auto onmovingstart = [this, locationInNode, tileCoord]()
                    {
                        spriteIsMoving = true;  //When the sprite begins moving, set spriteIsMoving to true
                        movesElapsed++;

                        if (!activePiece->isAscended() || tmxdat.checkSquareProperty(tileCoord, "Destroyable", _background) == "true") moveViewPointCenter(locationInNode,
                            [this](){screenIsMoving = true;  },
                            [this](){screenIsMoving = false; });
                        else
                        {
                            setViewPointCenter(locationInNode);
                            runAction(Shake::actionWithDuration(.6f, 5.5f));
                        }

                    };

                    auto onmovingend = [this, locationInNode]()
                    {
                        switchTeamTurn();
                        setActivePiece(nullptr);
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

                    
                    
                }
                else
                    setActivePiece(nullptr);
            }
        }

        return false; // if you are consuming it

    };

    auto _listener = EventListenerCustom::create("king taken", [=](EventCustom* event){
        _eventDispatcher->removeAllEventListeners();
        Director::getInstance()->replaceScene(TransitionFade::create(0.8, KnightWorld::createScene(), Color3B(0, 0, 0)));
    });


    _eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);

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
            lastPanId = gesture->getID(); 
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
    if (!spriteIsMoving && !screenIsMoving)
    {
        /*static int lastPinchId = -1;
        static bool pinchInsideNode = false;
        static float originalScale;
        static bool scaleset = false;

        // A new pinch
        if (gesture->getID() != lastPinchId)
        {
            lastPinchId = gesture->getID();
            originalScale = getScale();
            scaleset = true;
        }
        
        if (!scaleset)
        {

            setScale(gesture->getScale());
        }
        else
        {
            setScale(originalScale * gesture->getScale());
        }*/
            
    }
    
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

void KnightWorld::switchTeamTurn()
{
    if (movesElapsed >= movesPerTurn || activePiece->getPieceType() == TypeKing)
    {
        setCurTeam((getCurTeam() == TeamRed) ? TeamBlue : TeamRed);
        movesElapsed = 0;
        auto paren = " (" + tostring(movesPerTurn - movesElapsed) + ")";
        teamLabel->setString((getCurTeam() == TeamRed) ? "Red Team turn" + paren : "Blue Team turn" + paren);
        teamLabel->setColor((getCurTeam() == TeamRed) ? ccc3(255, 0, 0) : ccc3(0, 200, 255));
        teamLabel->setScale(1.4f);
        teamLabel->runAction(EaseOut::create(ScaleTo::create(.3f, 1.0f), 0.3f));
        totalTurnsPassed++;
        distributePowerUps();
        cleanupHoles();
    }
    else
    {
        auto paren = " (" + tostring(movesPerTurn - movesElapsed) + ")";
        teamLabel->setString((getCurTeam() == TeamRed) ? "Red Team turn" + paren : "Blue Team turn" + paren);
    }
}

void KnightWorld::distributePowerUps()
{
    if (totalTurnsPassed > 0)//3 + floor(CCRANDOM_0_1() * 2))
        for (int x = 0; x < tmxdat.tileswide; ++x)
            for (int y = 0; y < tmxdat.tilestall; ++y)
                if (tmxdat.checkSquareProperty(Vec2(x, y), "Piece type", _spawn) == "Powerup" && (tmxdat.getPieceInSquare(Vec2(x, y), pieces) == nullptr) && tmxdat.checkSquareProperty(Vec2(x, y), "Collideable", _background) != "true")
                    if (true)//floor(CCRANDOM_0_1() * 30) == 0) // TODO: Use random seed
                    {
                        auto curr = Powerup::create("imgs/powerup.png", tmxdat);
                        pieces.push_back(curr);
                        curr->setPosition(tmxdat.centerPositionForTileCoord(Vec2(x, y)));
                        addChild(curr, 20);
                        curr->setScale(1 / 1.3f);
                        curr->runAction(RepeatForever::create(Sequence::create(EaseInOut::create(ScaleBy::create(1.0f, 1.3f), 2.0f), EaseInOut::create(ScaleBy::create(1.0f, 1 / 1.3f), 2.0f), nullptr)));
                        curr->runAction(RepeatForever::create(RotateBy::create(10.0f, 360.0f)));
                    }
}

void KnightWorld::cleanupHoles()
{
    for (int x = 0; x < tmxdat.tileswide; ++x)
        for (int y = 0; y < tmxdat.tilestall; ++y)
            if (_background->tileGIDAt(Vec2(x, y)) == HoleSquare)
                if (floor(CCRANDOM_0_1() * 10) == 0)
                    _background->setTileGID(EmptySquare, Vec2(x, y));
}

void KnightWorld::setCurTeam(PieceTeam team)
{
    for (int i = 0; i < pieces.size(); i++)
        pieces[i]->endWiggle();
    currentTeamTurn = team;
    for (int i = 0; i < pieces.size(); i++)
        if (pieces[i]->getTeam() == getCurTeam() && (pieces[i]->getPieceType() != TypeKing || movesElapsed == 0))
            pieces[i]->beginWiggle();
}

void KnightWorld::initiateTeamText()
{
    TTFConfig ttfConfig("fonts/Munro.ttf", 60, GlyphCollection::NEHE);                                              // I have no idea how any of this works
    ttfConfig.fontFilePath = "fonts/Munro.ttf";                                                                     // I have no idea how any of this works
    teamLabel = Label::createWithTTF(ttfConfig, "fonts/Munro.ttf", TextHAlignment::CENTER, 0);                      // I have no idea how any of this works
    //teamLabel = Label::(createWithSystemFont("Red Team turn " + tostring(movesPerTurn - movesElapsed) + "", , 48);
    teamLabel->setString((currentTeamTurn == TeamRed ? "Red Team turn (" : "Blue Team turn (") + tostring(movesPerTurn - movesElapsed) + ")");
    teamLabel->setPosition(tmxdat.centerPositionForTileCoord(Vec2(tmxdat.tileswide / 2.0f, -1.0f)));
    teamLabel->enableShadow(Color4B(0, 0, 0, 150), Size(3, 3), 0);
    teamLabel->setColor(currentTeamTurn == TeamRed ? ccc3(255, 0, 0) : ccc3(0, 200, 255));
    this->addChild(teamLabel, 1);
}
void KnightWorld::setActivePiece(Piece* piece)
{
    if (piece != nullptr)
    {
        for (int i = 0; i < pieces.size(); i++)
            if (pieces[i] != piece)
                pieces[i]->endWiggle();
        activePiece = dynamic_cast<Knight *>(piece);
        initiateSquareWiggle();
    }
    else
    {
        activePiece = nullptr;
        setCurTeam(currentTeamTurn);
    }
}

void KnightWorld::initiateSquareWiggle()
{
    if (activePiece != nullptr)
    {
        auto squarestowiggle = activePiece->possibleSquaresToMoveOn(pieces);
        for (int i = 0; i < squarestowiggle.size(); i++)
        {
            if (tmxdat.tileCoordInMapBounds(squarestowiggle[i]))
                if (_background->tileAt(squarestowiggle[i])->numberOfRunningActions() == 0)
                    makeSpriteWiggle(_background->tileAt((squarestowiggle[i])));
        }
    }
}


void KnightWorld::makeSpriteWiggle(Sprite* square)
{
    if (activePiece != nullptr)
    {
        if (square->getAnchorPoint() != Vec2(0.5f, 0.5f))
        {
            square->setPosition(square->getPosition() + Vec2(tmxdat.tilewidth / 2, tmxdat.tileheight / 2));
            square->setAnchorPoint(Vec2(0.5f, 0.5f));
        }
        square->runAction(Sequence::create(
            EaseOut::create(RotateBy::create(.1f, 5.0f), 0.6f),
            EaseInOut::create(RotateBy::create(.2f, -10.0f), 2.0f),
            EaseIn::create(RotateBy::create(.1f, 5.0f), 0.6f),
            CCCallFunc::create([this, square](){makeSpriteWiggle(square); }),
            nullptr
            ));
    }

}

void KnightWorld::resetGame()
{
    _eventDispatcher->removeAllEventListeners();
    Director::getInstance()->replaceScene(TransitionFade::create(0.8, KnightWorld::createScene(), Color3B(0, 0, 0)));
}
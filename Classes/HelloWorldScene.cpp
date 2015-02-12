#include "HelloWorldScene.h"

using namespace std;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void addFrameToAnim(const std::string &frameName, Vector<SpriteFrame *> &animFrames, SpriteFrameCache *spritecache)
{
	auto frame = spritecache->getSpriteFrameByName(frameName);
	animFrames.insert(animFrames.size(), frame);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = LabelTTF::create("Hello World", "Arial", 24);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	// add "HelloWorld" splash screen"
	auto logo = Sprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(logo, 0);

	auto character = Sprite::create("imgs/sprite1.png");
	character->setPosition(Vec2(visibleSize.width / 3.5 + origin.x, visibleSize.height / 3.5 + origin.y));
	this->addChild(character, 0);
	auto ninjasquad = EaseBounceOut::create(MoveBy::create(2, Vec2(600, 200)));
	auto extremespeed = EaseElasticOut::create(MoveTo::create(4, Vec2(500, visibleSize.height / 2)));
	auto delay = DelayTime::create(1.25f);
	auto doublesize = ScaleBy::create(0.0f, 3.0f);

	auto seq = Sequence::create(ninjasquad, delay, extremespeed, nullptr);

	character->runAction(seq->clone());

	// Load smilegif
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("imgs/smiles.plist");
	auto smile = Sprite::createWithSpriteFrameName("frame1.png");
	smile->setPosition(Vec2(visibleSize.width / 1.5 + origin.x, visibleSize.height / 1.5 + origin.y));
	this->addChild(smile, 0);
	//smile->runAction(Sequence::create(doublesize, ninjasquad, delay, extremespeed, nullptr));

	Vector<SpriteFrame*> animFrames(3);
	addFrameToAnim("frame1.png", animFrames, spritecache);
	addFrameToAnim("frame2.png", animFrames, spritecache);
	addFrameToAnim("frame3.png", animFrames, spritecache);
	addFrameToAnim("frame4.png", animFrames, spritecache);
	addFrameToAnim("frame5.png", animFrames, spritecache);
	addFrameToAnim("frame5.png", animFrames, spritecache);
	addFrameToAnim("frame4.png", animFrames, spritecache);
	addFrameToAnim("frame3.png", animFrames, spritecache);
	addFrameToAnim("frame2.png", animFrames, spritecache);
	addFrameToAnim("frame1.png", animFrames, spritecache);
	Animation* smilemation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	smile->runAction(RepeatForever::create(Animate::create(smilemation)));

	//Create a "one by one" touch event listener (processes one touch at a time)
	auto listener1 = EventListenerTouchOneByOne::create();
	// When "swallow touches" is true, then returning 'true' from the onTouchBegan method will "swallow" the touch event, preventing other listeners from using it.
	listener1->setSwallowTouches(true);

	/// Example of using a lambda expression to implement onTouchBegan event callback function
	listener1->onTouchBegan = [animFrames](Touch* touch, Event* event){
		auto target = static_cast<Layer*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		auto spritecache = SpriteFrameCache::getInstance();
		auto smile = Sprite::createWithSpriteFrameName("frame1.png");
		smile->setPosition(locationInNode);
		target->addChild(smile, 0);
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		smile->runAction(RepeatForever::create(Animate::create(animation)));
		return true;
	};
	//Trigger when moving touch
	//listener1->onTouchMoved = [](Touch* touch, Event* event){
	//	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	//	//Move the position of current button sprite
	//	target->setPosition(target->getPosition() + touch->getDelta());
	//};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);




	return true;
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

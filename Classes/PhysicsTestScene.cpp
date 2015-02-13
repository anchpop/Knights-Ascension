#include "PhysicsTestScene.h"
#include "VisibleRect.h"
#include "PEShapeCache_X3_0.h"

using namespace std;

USING_NS_CC;

Scene* HelloPhysics::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	// 'layer' is an autorelease object
	auto layer = HelloPhysics::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloPhysics::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto s_centre = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	auto _emitter = ParticleSun::create();
	_emitter->retain();

	_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("imgs/fire.png"));

	
	PEShapeCache::getInstance()->addBodysWithFile("physics/sprite1.plist");

	// wall
	auto wall = Node::create();
	wall->setPhysicsBody(PhysicsBody::createEdgeBox(Director::getInstance()->getVisibleSize(), PhysicsMaterial(0.1f, 1, 0.0f)));//VisibleRect::getVisibleRect().size, PhysicsMaterial(0.1f, 1, 0.0f)));
	wall->setPosition(VisibleRect::center());
	this->addChild(wall);


	//create a sprite
	auto sprite = Sprite::create("imgs/sprite1.png");
	sprite->setPosition(s_centre);
	addChild(sprite, 20);

	//apply physicsBody to the sprite
	PEShapeCache::getInstance()->addBodysWithFile("physics/sprite1.plist");
	auto spriteBody = PEShapeCache::getInstance()->getPhysicsBodyByName("sprite1"); // the name you put in PhysicsEditor
	spriteBody->setDynamic(false);
	sprite->setPhysicsBody(spriteBody);

	auto physicsBody = PhysicsBody::createBox(Size(65.0f, 81.0f),
		PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(true);
	//sprite->setPhysicsBody(physicsBody);



	//add five dynamic bodies
	for (int i = 0; i < 5; ++i)
	{
		physicsBody = PhysicsBody::createBox(Size(50.0f, 50.0f),
			PhysicsMaterial(0.1f, 1.0f, 0.0f));

		//set the body isn't is affected by the physics world's gravitational force
		physicsBody->setGravityEnable(false);

		//set initial velocity of physicsBody
		physicsBody->setVelocity(Vec2(cocos2d::random(-500, 500),
			cocos2d::random(-500, 500)));
		//physicsBody->setTag(DRAG_BODYS_TAG);

		sprite = Sprite::create("imgs/bouncy.png");
		auto position = Vec2(s_centre.x + cocos2d::random(-300, 300),
			s_centre.y + cocos2d::random(-300, 300));
		sprite->setPosition(position);
		sprite->setPhysicsBody(physicsBody);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));

		addChild(sprite);

		auto _emitter1 = ParticleSun::create();
		_emitter1->retain();

		_emitter1->setTexture(Director::getInstance()->getTextureCache()->addImage("imgs/fire.png"));
		_emitter1->setAnchorPoint(Vec2(0.5f, 0.5f));
		sprite->addChild(_emitter1, -1);
		_emitter1->setPosition(ccp(0, 25));

		auto _emitter2 = ParticleSun::create();
		_emitter2->retain();

		_emitter2->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("imgs/fire.png"));
		_emitter2->setAnchorPoint(Vec2(0.5f, 0.5f));
		sprite->addChild(_emitter2, -1);
		_emitter2->setPosition(ccp(50, 25));
	}
	

	return true;
}
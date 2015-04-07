// Programmed in Cocos2D-X 3.3 by Andre Popovitch, with are by Zeph Balsley

#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "PhysicsTestScene.h"
#include "maine.h"
#include "PEShapeCache_X3_0.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Knight's Ascension");
		glview->setFrameSize(1920/2, 1200/2);     // Dimensions of the Nexus 7 / 2
		//glview->setFrameZoomFactor(2.5f); 
		director->setOpenGLView(glview);
    }



    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    auto audio = CocosDenshion::SimpleAudioEngine::sharedEngine();

    // pre-loading background music and effects. You could pre-load
    // effects, perhaps on app startup so they are already loaded
    // when you want to use them.
    audio->preloadBackgroundMusic("sound/candy.mp3");

    // create a scene. it's an autorelease object
	auto scene = KnightWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

#pragma once

#include "cocos2d.h"
#include "Piece.h"
#include "Knight.h"
#include "KnightK.h"
#include "Powerup.h"
#include "TileUtils.h"
#include "TGestureRecognizer.h"
#include "Utils.h"
#include "VisibleRect.h"
#include <string>
#include <cstdio>
using namespace cocos2d;


class KnightWorld : public cocos2d::Layer, public TGestureHandler
{
protected:
	TGestureRecognizer* _gestureRecognizer;
private:
	TMXTiledMap *_tileMap;
	TMXLayer *_background;
	Knight *_player;
	TMXLayer *_spawn;
	TileMapTools tmxdat;

	double boardRPM;

	vector<Piece*> pieces;
	Knight* activePiece;

	bool spriteIsMoving;
	bool screenIsMoving;

	PieceTeam currentTeamTurn;
	int movesElapsed;
    int movesPerTurn;
    int totalTurnsPassed;

	Label* teamLabel;

    void switchTeamTurn();

	void initGestureRecognizer();

    void distributePowerUps();

    string tostring(int i) {char buff[33];
    sprintf(buff, "%d", i);
    return std::string(buff); }

    bool allPiecesShouldWiggle;

    Sprite* bg;

public:

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	Point getPointToCenterOn(Point position);

	void setViewPointCenter(Point position);



	void moveViewPointCenter(Point position, const std::function<void()>&, float time = 0.4f, float easing = 0.5f);

	void moveViewPointCenter(Point position, const std::function<void()>&, const std::function<void()>&, float time = 0.4f, float easing = 0.5f);

    void cleanupHoles();

	virtual bool onGesturePan(TGesturePan* gesture);
	virtual bool onGesturePinch(TGesturePinch* gesture);

	// Touch handler (pass touches to the Gesture recognizer to process)
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);
	virtual void TouchCancelled(Touch* touch, Event* event);

    PieceTeam getCurTeam() { return currentTeamTurn; }
    void setCurTeam(PieceTeam);
    void setActivePiece(Piece*);
    void makeSpriteWiggle(Sprite*);
    void initiateSquareWiggle();


	// implement the "static create()" method manually
	CREATE_FUNC(KnightWorld);
};


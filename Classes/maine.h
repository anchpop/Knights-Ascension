#pragma once

#include "cocos2d.h"
#include "Piece.h"
#include "Knight.h"
#include "TileUtils.h"
using namespace cocos2d;


class KnightWorld : public cocos2d::Layer
{
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




	// implement the "static create()" method manually
	CREATE_FUNC(KnightWorld);
};


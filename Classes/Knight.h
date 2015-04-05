#pragma once

#include "Piece.h"
#include "TileUtils.h"
using namespace cocos2d;
using namespace std;

class Knight : public Piece
{
private:
	TMXTiledMap *_tileMap;
	TMXLayer *_background;
	TMXLayer *_meta;
	TileMapTools _tmxdat;
	PieceType pieceType;
	PieceTeam team;
	std::vector<Vec2> relativePossibleKnightSquaresToMoveOn();
	std::vector<Vec2> relativePossibleBlockSquaresToMoveOn();
public:
	
	void setKnightPosition(Point position, const std::function<void()>&, const std::function<void()>&);
	//void setup(String, TMXTiledMap*, TMXLayer*, TMXLayer*, int, int);

	Knight(const std::string&, TMXTiledMap*, TMXLayer*, TileMapTools);
	~Knight();
	static Knight* create(const std::string&, TMXTiledMap*, TMXLayer*, TileMapTools);
	//static Knight* create(const std::string&, const std::string&, TileMapTools);
	void initOptions();


	std::vector<Vec2> possibleSquaresToMoveOn();
	std::string checkSquareProperty(Vec2 square, const string&);
};


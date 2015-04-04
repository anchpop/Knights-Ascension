#pragma once

#include "cocos2d.h"

enum PieceType {
	TypeKnight, TypeKing, TypePowerup
};

enum PieceTeam {
	TeamA, TeamB, Neutral
};

class Piece : public cocos2d::Sprite
{
public:
	Piece();
	~Piece();
	static Piece* create();

	void initOptions();

	PieceType pieceType;
	PieceTeam pieceTeam;


private:

};

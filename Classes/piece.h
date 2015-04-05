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
private:
	PieceType pieceType;
	PieceTeam team;
public:
	Piece();
	~Piece();
	static Piece* create();

	void initOptions();



	void setTeam(PieceTeam);

	PieceTeam getTeam();
	PieceType getPieceType();
};

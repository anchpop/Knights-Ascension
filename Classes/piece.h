#pragma once

#include "cocos2d.h"

enum PieceType {
	TypeKnight, TypeKing, TypePowerup
};

enum PieceTeam {
	TeamRed, TeamBlue, Neutral
};

class Piece : public cocos2d::Sprite
{
protected:
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

    void take(std::vector<Piece*>&);

	void getTaken();
};

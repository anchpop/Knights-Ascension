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
    
    void startWiggle();
    void restartWiggle();
public:
	Piece();
	~Piece();
	static Piece* create();

	void initOptions();


	void setTeam(PieceTeam);

	PieceTeam getTeam();
	PieceType getPieceType();

    virtual void take(std::vector<Piece*>&, Piece*);

    bool shouldWiggle;
    bool wiggleingHasStopped;
    void beginWiggle() { shouldWiggle = true; if (wiggleingHasStopped) startWiggle(); };
    void endWiggle() { shouldWiggle = false; };
};

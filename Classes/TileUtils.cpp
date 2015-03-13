#include "TileUtils.h"


int tilestall = 14;
int tileswide = 14;
int tileheight = 64;
int tilewidth = 64;

Point tileCoordForPosition(Point position)
{
	int x = position.x / tilewidth;
	int y = ((tilestall * tileheight) - position.y) / tileheight;
	return ccp(x, y);
}

Point positionForTileCoord(Point position)
{
	int x = position.x * tilewidth;
	int y = (tilestall - position.y) * tileheight;
	return ccp(x, y);
}

Point centerPositionForTileCoord(Point position)
{
	int x = position.x * tilewidth + (tilewidth / 2);
	int y = (tilestall - position.y) * tileheight - (tileheight / 2);
	return ccp(x, y);
}


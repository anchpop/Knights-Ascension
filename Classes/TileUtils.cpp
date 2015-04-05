#include "TileUtils.h"


TileMapTools::TileMapTools()
{
	//use default values
	tilestall = 14;
	tileswide = 14;
	tileheight = 64;
	tilewidth = 64;
}

TileMapTools::TileMapTools(int tilestall, int tileswide, int tileheight, int tilewidth)
{
	tilestall = tilestall;
	tileswide = tileswide;
	tileheight = tileheight;
	tilewidth = tilewidth;
}


Point TileMapTools::tileCoordForPosition(const Point& position)
{
	int x = position.x / tilewidth;
	int y = ((tilestall * tileheight) - position.y) / tileheight;
	return Vec2(x, y);
}

Point TileMapTools::positionForTileCoord(const Point& position)
{
	int x = position.x * tilewidth;
	int y = (tilestall - position.y) * tileheight;
	return Vec2(x, y);
}

Point TileMapTools::centerPositionForTileCoord(const Point& position)
{
	int x = position.x * tilewidth + (tilewidth / 2);
	int y = (tilestall - position.y) * tileheight - (tileheight / 2);
	return Vec2(x, y);
}

Point TileMapTools::roundedPosition(const Point& position)
{
	return positionForTileCoord(tileCoordForPosition(position));
}

Point TileMapTools::roundedCenterPosition(const Point& position)
{
	return centerPositionForTileCoord(tileCoordForPosition(position));
}

bool TileMapTools::tileCoordInMapBounds(const Point& position)
{
	return !(position.x < 0 || position.y < 0 || position.x >= tileswide || position.y >= tilestall);
}

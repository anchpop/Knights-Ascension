#include "TileUtils.h"

TileMapTools::TileMapTools() { }



TileMapTools::TileMapTools(TMXTiledMap* _map, std::vector<Piece*> _pieces)
{
	//use default values
	tilestall = 16;
	tileswide = 15;
	tileheight = 64;
	tilewidth = 64;
	map = _map;
	_background = map->layerNamed("mainboard");
	pieces = _pieces;
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


std::string TileMapTools::checkSquareProperty(Vec2 square, const std::string &property, TMXLayer* layer)
{
	if (tileCoordInMapBounds(square))
	{
		int tileGid = layer->tileGIDAt(square);
		if (tileGid) {
			auto properties = map->propertiesForGID(tileGid).asValueMap();
			if (!properties.empty()) {
				auto propstring = properties[property].asString();
				return propstring;
			}
		}
	}
	return "";
}
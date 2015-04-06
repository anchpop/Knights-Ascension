#include "TileUtils.h"


TileMapTools::TileMapTools() { }


TileMapTools::TileMapTools(TMXTiledMap* map) : _map(map)
{
    //use default values
    _background = _map->layerNamed("mainboard");
}


Point TileMapTools::tileCoordForPosition(const Point& position, bool yflip)
{
    int x = position.x / tilewidth;
    int y = yflip ? ((tilestall * tileheight) - position.y) / tileheight : position.y / tileheight;
    return Vec2(x, y);
}

Point TileMapTools::positionForTileCoord(const Point& position, bool yflip)
{
    int x = position.x * tilewidth;
    int y = (yflip ? (tilestall - position.y) : position.y) * tileheight;
    return Vec2(x, y);
}

Point TileMapTools::centerPositionForTileCoord(const Point& position, bool yflip)
{
    int x = position.x * tilewidth + (tilewidth / 2);
    int y = (yflip ? (tilestall - position.y) : position.y) * tileheight - (tileheight / 2);
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
        if (tileGid) 
        {
            auto properties = _map->propertiesForGID(tileGid).asValueMap();
            if (!properties.empty())
            {
                auto propstring = properties[property].asString();
                return propstring;
            }
        }
    }
    return "";
}

Piece* TileMapTools::getPieceInSquare(const Vec2& position, std::vector<Piece *>& pieces)
{
    for (int x = 0; x < pieces.size(); ++x)
    {
        auto inputpos = centerPositionForTileCoord(position);
        auto piece = pieces[x]->getPosition();
        if (inputpos.x == piece.x && inputpos.y == piece.y)
            return pieces[x];
    }
    return nullptr;
}

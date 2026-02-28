/*********************************************************************
(c) Matt Marchant 2024
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include "MapLayer.hpp"

#include <tmxlite/TileLayer.hpp>

#include <cassert>

MapLayer::MapLayer()
{

}

bool MapLayer::create(const tmx::Map& map, std::uint32_t layerIndex, const std::vector<std::unique_ptr<Texture>>& textures)
{
    const auto& layers = map.getLayers();
    assert(layers[layerIndex]->getType() == tmx::Layer::Type::Tile);
    
    const auto& layer = layers[layerIndex]->getLayerAs<tmx::TileLayer>();
    const auto mapSize = map.getTileCount();
    const auto mapTileSize = map.getTileSize();
    const auto& tileSets = map.getTilesets();

    m_mapPixelWidth = static_cast<float>(mapSize.x * mapTileSize.x);
    m_mapPixelHeight = static_cast<float>(mapSize.y * mapTileSize.y);

    const auto tintColour = layer.getTintColour();

    const SDL_FColor vertColour = SDL_FColor{ tintColour.r / 255.0f, tintColour.g / 255.0f, tintColour.b / 255.0f, tintColour.a / 255.0f };

    for (auto i = 0u; i < tileSets.size(); ++i)
    {
        SDL_Texture* sdlTexture = static_cast<SDL_Texture*>(*textures[i]);
        if (!sdlTexture)
        {
            continue;
        }

        //check tile ID to see if it falls within the current tile set
        const auto& ts = tileSets[i];
        const auto& tileIDs = layer.getTiles();

        const auto texSize = textures[i]->getSize();
        const auto tileCountX = texSize.x / mapTileSize.x;
        const auto tileCountY = texSize.y / mapTileSize.y;

        const float uNorm = static_cast<float>(mapTileSize.x) / texSize.x;
        const float vNorm = static_cast<float>(mapTileSize.y) / texSize.y;

        std::vector<SDL_Vertex> verts;
        for (auto y = 0u; y < mapSize.y; ++y)
        {
            for (auto x = 0u; x < mapSize.x; ++x)
            {
                const auto idx = y * mapSize.x + x;
                if (idx < tileIDs.size() && tileIDs[idx].ID >= ts.getFirstGID()
                    && tileIDs[idx].ID < (ts.getFirstGID() + ts.getTileCount()))
                {
                    //tex coords
                    auto idIndex = (tileIDs[idx].ID - ts.getFirstGID());
                    float u = static_cast<float>(idIndex % tileCountX);
                    float v = static_cast<float>(idIndex / tileCountX);
                    u *= mapTileSize.x; //TODO we should be using the tile set size, as this may be different from the map's grid size
                    v *= mapTileSize.y;

                    //normalise the UV
                    u /= textures[i]->getSize().x;
                    v /= textures[i]->getSize().y;

                    //vert pos
                    const float tilePosX = static_cast<float>(x) * mapTileSize.x;
                    const float tilePosY = (static_cast<float>(y) * mapTileSize.y);

                    // Handle tile flipping
                    const std::uint8_t flipFlags = tileIDs[idx].flipFlags;
                    const bool flipH = (flipFlags & tmx::TileLayer::Horizontal) != 0;
                    const bool flipV = (flipFlags & tmx::TileLayer::Vertical) != 0;
                    const bool flipD = (flipFlags & tmx::TileLayer::Diagonal) != 0;

                    // UV corners: adjust based on flip flags
                    float u0 = u, u1 = u + uNorm;
                    float v0 = v, v1 = v + vNorm;

                    if (flipD)
                    {
                        // Diagonal flip (rotate 90° clockwise)
                        if (flipH && flipV)
                        {
                            // 90° CCW rotation
                            std::swap(u0, u1);
                        }
                        else if (flipH)
                        {
                            // 90° CW rotation + horizontal flip = 270° CW
                            std::swap(v0, v1);
                        }
                        else if (flipV)
                        {
                            // 90° CW rotation + vertical flip = 90° CCW
                            std::swap(u0, u1);
                        }
                        // Note: diagonal flip transforms require UV transposition
                    }
                    else
                    {
                        if (flipH) std::swap(u0, u1);
                        if (flipV) std::swap(v0, v1);
                    }

                    //push back to vert array (two triangles forming a quad)
                    SDL_Vertex vert = { { tilePosX, tilePosY }, vertColour, {u0, v0} };
                    verts.emplace_back(vert);
                    vert = { { tilePosX + mapTileSize.x, tilePosY }, vertColour, {u1, v0} };
                    verts.emplace_back(vert);
                    vert = { { tilePosX, tilePosY + mapTileSize.y}, vertColour, {u0, v1} };
                    verts.emplace_back(vert);
                    
                    vert = { { tilePosX, tilePosY + mapTileSize.y}, vertColour, {u0, v1} };
                    verts.emplace_back(vert);
                    vert = { { tilePosX + mapTileSize.x, tilePosY }, vertColour, {u1, v0} };
                    verts.emplace_back(vert);
                    vert = { { tilePosX + mapTileSize.x, tilePosY + mapTileSize.y }, vertColour, {u1, v1} };
                    verts.emplace_back(vert);
                }
            }
        }

        if (!verts.empty())
        {
            m_subsets.emplace_back();
            m_subsets.back().texture = sdlTexture;
            m_subsets.back().vertexData.swap(verts);
        }
    }

    return true;
}

void MapLayer::draw(SDL_Renderer* renderer, const float scale ) const
{
    assert( renderer );

    for( const auto& s : m_subsets )
    {
        std::vector<SDL_Vertex> scaledVerts;
        scaledVerts.reserve( s.vertexData.size() );

        for( const auto& v : s.vertexData )
        {
            SDL_Vertex out = v;
            out.position.x *= scale;
            out.position.y *= scale;
            scaledVerts.emplace_back(out);
        }
    
        SDL_RenderGeometry( renderer, s.texture, scaledVerts.data(), static_cast<int>( scaledVerts.size() ), nullptr, 0);
    }
}

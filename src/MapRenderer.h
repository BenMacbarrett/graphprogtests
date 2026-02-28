#pragma once

#include "MapLayer.hpp"
#include <iostream>

class MapRenderer
{
    tmx::Map map;
    std::vector<std::unique_ptr<Texture>> textures;
    std::vector<std::unique_ptr<MapLayer>> renderLayers;
    int mapPixelWidth = 0;
    int mapPixelHeight = 0;
    
public:
    MapRenderer(){}
    ~MapRenderer(){}
    
    int getMapPixelWidth() const { return mapPixelWidth; }
    int getMapPixelHeight() const { return mapPixelHeight; }

    bool loadMap( SDL_Renderer* renderer, const std::string& path)
    {
        if (map.load(path))
        {
            auto mapSize = map.getTileCount();
            auto tileSize = map.getTileSize();
            
            mapPixelWidth = mapSize.x * tileSize.x;
            mapPixelHeight = mapSize.y * tileSize.y;
            
            //load the textures as they're shared between layers
            const auto& tileSets = map.getTilesets();
            assert(!tileSets.empty());
            for (const auto& ts : tileSets)
            {
                textures.emplace_back(std::make_unique<Texture>());
                if( !textures.back()->loadFromFile(ts.getImagePath(), renderer ) )
                {
                    std::cerr << "Failed opening " << ts.getImagePath() << "\n";
                }
            }

            //load the layers
            const auto& mapLayers = map.getLayers();
            for (auto i = 0u; i < mapLayers.size(); ++i)
            {
                if (mapLayers[i]->getType() == tmx::Layer::Type::Tile)
                {
                    std::cout << "ICI \n";
                    renderLayers.emplace_back(std::make_unique<MapLayer>());
                    renderLayers.back()->create(map, i, textures); //just cos we're using C++14
                }
            }
        }

        return true;
    }

    void draw( SDL_Renderer* renderer, const float scale ) const
    {
        for( const auto& l : renderLayers )
        {
            l->draw( renderer, scale );
        }
    }

};
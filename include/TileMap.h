#pragma once

#include "Entity.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    struct PrintControl_s
    {
        std::string TileSet;
        sf::Vector2<float> TileSize;
        sf::Vector2<float> PrintSize;
        Entity::Position_s Position;
        
        unsigned short Tile;
        unsigned int ImgY;
        bool bInverted;
        bool bObstacle;
        
        PrintControl_s()
        : Tile(0), ImgY(0), bInverted(0), TileSize(0, 0), PrintSize(0, 0), bObstacle(true) {}
    };
    
public:
    bool Load(const PrintControl_s PrintControl);

    TileMap();
    ~TileMap();

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    sf::VertexArray m_Vertices;
    sf::Texture m_TileSet;
};

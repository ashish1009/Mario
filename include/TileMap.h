#pragma once
#include "Entity.h"

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    struct PrintControl_s {
        std::string tileSet;            /// Image Path of Entity
        sf::Vector2f tileSize;          ///
        Entity::Position_s position;    /// Position of Entity where Entity need to be printed
        
        short tile;                     /// Index of image in X direction that need to be printed
        short imgY;                     /// Index of Image in Y direction that need to be printed
        bool bInverted;                 /// flag to check if Image need to be inverted
        
        sf::Vector2<short> blockBreakPart;           /// if Block need to be break then store the part number else 0
                                        /// ________
                                        /// | 0,0   0,1 |
                                        /// | 1,0   1,1 |
                                        /// |-------------|
        PrintControl_s()
        : tile(0), imgY(0), bInverted(false), blockBreakPart(0, 0) {}
    };

public:
    bool Load(const PrintControl_s &printControl);

    TileMap();
    ~TileMap();

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    sf::VertexArray m_Vertices;
    sf::Texture m_TileSet;
};

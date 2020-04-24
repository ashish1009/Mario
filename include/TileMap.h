#pragma once
#include <SFML/Graphics.hpp>
#include "Common.h"

struct Position_s {
    float X;
    float Y;
    
    Position_s();
    Position_s(const float x, const float y);
};

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    enum ImageTileType_e : short {
        ITEM = 0,
        PLAYER = 1,
        ENEMY = 2
    };

    struct PrintControl_s {
        bool bIsInverted;                     /// flag to check if Image need to be inverted
        short imgXIdx;                      /// Index of image in X direction that need to be printed : NOTE : Index should be accordimg to tileoffset
        short imgYIdx;                      /// Index of Image in Y direction that need to be printed : NOTE It is exact Index of Image in Y Direction No Offset needed
        short xTileOffset;                  /// Offset in both x and y direction of the image in tha larger image that need to be printed
        
        ImageTileType_e imgType;            /// Image Path of Entity
        Position_s position;                /// Position of Entity where Entity need to be printed
        sf::Vector2<float> tileSize;        /// Size that need to be printed from Image from ImgX and imgY
        
        PrintControl_s()
        : bIsInverted(false), imgXIdx(0), imgYIdx(0), imgType(ITEM) {}
    };
    
public:
    void Load(PrintControl_s printControl);

    TileMap();
    ~TileMap();

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    
    ImageTileType_e m_ImageTileType;
    
    sf::VertexArray m_Vertices;
    sf::Texture m_IconTileSet;
    sf::Texture m_PlayerTileSet;
    sf::Texture m_EnemyTileSet;
    sf::Texture *m_TileSet;
};

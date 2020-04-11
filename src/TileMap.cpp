#include "TileMap.h"
#include "Logger.h"

#define QUAD_VERTEX 4

bool TileMap::Load(const PrintControl_s &printControl) {
    const int X = printControl.position.X;
    const int Y = printControl.position.Y;
        
    if (!m_TileSet.loadFromFile(printControl.tileSet)) {
        LogError(BIT_TILEMAP, "TileMap::LoadPlayer(), Can Not Load Image \n");
        return false;
    }

    m_Vertices.setPrimitiveType(sf::Quads);
    m_Vertices.resize(QUAD_VERTEX);
    
    int imgX = printControl.tile % (int)(m_TileSet.getSize().x >> BLOCK_SIZE_BIT);
    
    m_Vertices[0].position = sf::Vector2f(X, (Y - printControl.tileSize.y));
    m_Vertices[1].position = sf::Vector2f((X + printControl.tileSize.x), (Y - printControl.tileSize.y));
    m_Vertices[2].position = sf::Vector2f((X + printControl.tileSize.x), Y);
    m_Vertices[3].position = sf::Vector2f(X, Y);
    
    if (!printControl.bInverted) {
        m_Vertices[0].texCoords = sf::Vector2f((imgX << BLOCK_SIZE_BIT) + (printControl.blockBreakPart.x << 3), printControl.imgY  + (printControl.blockBreakPart.y << 3));
        m_Vertices[1].texCoords = sf::Vector2f(((imgX << BLOCK_SIZE_BIT) + printControl.tileSize.x)  + (printControl.blockBreakPart.x << 3), printControl.imgY  + (printControl.blockBreakPart.y << 3));
        m_Vertices[2].texCoords = sf::Vector2f(((imgX << BLOCK_SIZE_BIT) + printControl.tileSize.x)  + (printControl.blockBreakPart.x << 3), (printControl.imgY + printControl.tileSize.y)  + (printControl.blockBreakPart.y << 3));
        m_Vertices[3].texCoords = sf::Vector2f((imgX << BLOCK_SIZE_BIT)  + (printControl.blockBreakPart.x << 3), (printControl.imgY + printControl.tileSize.y)  + (printControl.blockBreakPart.y << 3));
    }
    else {
        m_Vertices[0].texCoords = sf::Vector2f(((imgX << BLOCK_SIZE_BIT) + printControl.tileSize.x), printControl.imgY);
        m_Vertices[1].texCoords = sf::Vector2f((imgX << BLOCK_SIZE_BIT), printControl.imgY);
        m_Vertices[2].texCoords = sf::Vector2f((imgX << BLOCK_SIZE_BIT), printControl.imgY + printControl.tileSize.y);
        m_Vertices[3].texCoords = sf::Vector2f(((imgX << BLOCK_SIZE_BIT) + printControl.tileSize.x), printControl.imgY + printControl.tileSize.y);
    }
    return true;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &m_TileSet;
    target.draw(m_Vertices, states);
}

TileMap::TileMap() {
    //    LogDebug(BIT_TILEMAP, "TileMap::TileMap(), Constructor called !!\n");
}

TileMap::~TileMap() {
    //    LogDebug(BIT_TILEMAP, "TileMap::~TileMap(), Destructor called !!\n");
}



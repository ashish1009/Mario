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
    
    int imgX = printControl.tile % (int)(m_TileSet.getSize().x / printControl.tileSize.x);
    
    m_Vertices[0].position = sf::Vector2f(X, (Y - printControl.tileSize.y));
    m_Vertices[1].position = sf::Vector2f((X + printControl.tileSize.x), (Y - printControl.tileSize.y));
    m_Vertices[2].position = sf::Vector2f((X + printControl.tileSize.x), Y);
    m_Vertices[3].position = sf::Vector2f(X, Y);
    
    if (!printControl.bInverted) {
        m_Vertices[0].texCoords = sf::Vector2f((imgX * printControl.tileSize.x), printControl.imgY);
        m_Vertices[1].texCoords = sf::Vector2f(((imgX + 1) * printControl.tileSize.x), printControl.imgY);
        m_Vertices[2].texCoords = sf::Vector2f(((imgX + 1) * printControl.tileSize.x), (printControl.imgY + printControl.tileSize.y));
        m_Vertices[3].texCoords = sf::Vector2f((imgX * printControl.tileSize.x), (printControl.imgY + printControl.tileSize.y));
    }
    else {
        m_Vertices[0].texCoords = sf::Vector2f(((imgX + 1) * printControl.tileSize.x), printControl.imgY);
        m_Vertices[1].texCoords = sf::Vector2f((imgX * printControl.tileSize.x), printControl.imgY);
        m_Vertices[2].texCoords = sf::Vector2f((imgX * printControl.tileSize.x), printControl.imgY + printControl.tileSize.y);
        m_Vertices[3].texCoords = sf::Vector2f(((imgX + 1) * printControl.tileSize.x), printControl.imgY + printControl.tileSize.y);
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



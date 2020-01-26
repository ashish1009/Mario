#include "TileMap.h"
#include "Player.h"
#include "Logger.h"

#define QUAD_VERTEX 4

bool TileMap::Load(const PrintControl_s PrintControl)
{
    float X = (float)(PrintControl.Position.X * 1000) / WIN_WIDTH;
    float Y = (float)(PrintControl.Position.Y * 1000) / WIN_HEIGHT;
    
    if (!m_TileSet.loadFromFile(PrintControl.TileSet))
    {
        LogError(BIT_TILEMAP, "TileMap::LoadPlayer(), Can Not Load Image \n");
        return false;
    }

    m_Vertices.setPrimitiveType(sf::Quads);
    m_Vertices.resize(QUAD_VERTEX);

    int ImgX = PrintControl.Tile % (int)(m_TileSet.getSize().x / PrintControl.TileSize.x);

    m_Vertices[0].position = sf::Vector2f(X, (Y - PrintControl.PrintSize.y));
    m_Vertices[1].position = sf::Vector2f((X + PrintControl.PrintSize.x), (Y - PrintControl.PrintSize.y));
    m_Vertices[2].position = sf::Vector2f((X + PrintControl.PrintSize.x), Y);
    m_Vertices[3].position = sf::Vector2f(X, Y);
    
    if (!PrintControl.bInverted)
    {
        m_Vertices[0].texCoords = sf::Vector2f((ImgX * PrintControl.TileSize.x), PrintControl.ImgY);
        m_Vertices[1].texCoords = sf::Vector2f(((ImgX + 1) * PrintControl.TileSize.x), PrintControl.ImgY);
        m_Vertices[2].texCoords = sf::Vector2f(((ImgX + 1) * PrintControl.TileSize.x), (PrintControl.ImgY + PrintControl.TileSize.y));
        m_Vertices[3].texCoords = sf::Vector2f((ImgX * PrintControl.TileSize.x), (PrintControl.ImgY + PrintControl.TileSize.y));
    }
    else
    {
        m_Vertices[0].texCoords = sf::Vector2f(((ImgX + 1) * PrintControl.TileSize.x), PrintControl.ImgY);
        m_Vertices[1].texCoords = sf::Vector2f((ImgX * PrintControl.TileSize.x), PrintControl.ImgY);
        m_Vertices[2].texCoords = sf::Vector2f((ImgX * PrintControl.TileSize.x), PrintControl.ImgY + PrintControl.TileSize.y);
        m_Vertices[3].texCoords = sf::Vector2f(((ImgX + 1) * PrintControl.TileSize.x), PrintControl.ImgY + PrintControl.TileSize.y);
    }
    return true;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_TileSet;
    target.draw(m_Vertices, states);
}

TileMap::TileMap()
{
    //    LogDebug(BIT_TILEMAP, "TileMap::TileMap(), Constructor called !!\n");
}

TileMap::~TileMap()
{
    //    LogDebug(BIT_TILEMAP, "TileMap::~TileMap(), Destructor called !!\n");
}

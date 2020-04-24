#include "TileMap.h"
#include "Logger.h"

#define QUAD_VERTEX 4

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Load the image according to parameters printControl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TileMap::Load(PrintControl_s printControl) {
    switch (printControl.imgType) {
        case ITEM:
            m_TileSet = &m_IconTileSet;
            break;
            
        case PLAYER:
            m_TileSet = &m_PlayerTileSet;
//            std::cout << std::dec << printControl.position.X << " " << printControl.position.Y << "\n";
            break;
            
        case ENEMY:
            m_TileSet = &m_EnemyTileSet;
            break;
            
        default:
            LogError( BIT_TILEMAP, "TileMap::Load(),  Invalid Image Type %d \n", printControl.imgType);
            break;
    }
    
    const float &X = printControl.position.X;
    const float &Y = printControl.position.Y;
    const short &imgY = printControl.imgYIdx;
    const short imgX = (printControl.imgXIdx % static_cast<short>((*m_TileSet).getSize().x >> BLOCK_SIZE_BIT)) << printControl.xTileOffset;

    m_Vertices.setPrimitiveType(sf::Quads);
    m_Vertices.resize(QUAD_VERTEX);
    
    m_Vertices[0].position = sf::Vector2f(X, (Y - printControl.tileSize.y));
    m_Vertices[1].position = sf::Vector2f((X + printControl.tileSize.x), (Y - printControl.tileSize.y));
    m_Vertices[2].position = sf::Vector2f((X + printControl.tileSize.x), Y);
    m_Vertices[3].position = sf::Vector2f(X, Y);

    if (!printControl.bIsInverted) {
        m_Vertices[0].texCoords = sf::Vector2f(imgX, imgY);
        m_Vertices[1].texCoords = sf::Vector2f((imgX + printControl.tileSize.x), imgY);
        m_Vertices[2].texCoords = sf::Vector2f((imgX + printControl.tileSize.x), (imgY + printControl.tileSize.y));
        m_Vertices[3].texCoords = sf::Vector2f(imgX, (imgY + printControl.tileSize.y));
    }
    else {
        m_Vertices[0].texCoords = sf::Vector2f(imgX + printControl.tileSize.x, imgY);
        m_Vertices[1].texCoords = sf::Vector2f((imgX), imgY);
        m_Vertices[2].texCoords = sf::Vector2f((imgX), (imgY + printControl.tileSize.y));
        m_Vertices[3].texCoords = sf::Vector2f(imgX + printControl.tileSize.x, (imgY + printControl.tileSize.y));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Print the loaded image
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = m_TileSet;
    target.draw(m_Vertices, states);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TileMap::TileMap()
: m_TileSet(nullptr) {
    if (!m_IconTileSet.loadFromFile(gResourcePath + "graphics/item_objects.png")) {
        LogError(BIT_TILEMAP, "TileMap::TileMap(), Can Not Load Image \n");
        return;
    }
    
    if (!m_PlayerTileSet.loadFromFile(gResourcePath + "graphics/Mario.png")) {
        LogError(BIT_TILEMAP, "TileMap::TileMap(), Can Not Load Image \n");
        return;
    }
    
    if (!m_EnemyTileSet.loadFromFile(gResourcePath + "graphics/Enemies.png")) {
        LogError(BIT_TILEMAP, "TileMap::TileMap(), Can Not Load Image \n");
        return;
    }
//    LogInfo(BIT_TILEMAP, "TileMap::TileMap(): Constructor called !!!\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TileMap::~TileMap() {
//    LogInfo(BIT_TILEMAP, "TileMap::~TileMap(): Destructor called !!!\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Struct Positin_s Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Position_s::Position_s()
: X(0), Y(0) {
    
}

Position_s::Position_s(const float x, const float y)
: X(x), Y(y) {
    
}

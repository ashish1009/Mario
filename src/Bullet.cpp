#include "Bullet.h"
#include "Logger.h"
//#include "Obstacle.h"

const short gExplosionTime = 6;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Constructor
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Bullet::Bullet()
: m_BulletY(0), m_BulletX(0), m_ExplosionCounter(0) {
//    LogInfo(BIT_BONUS, "Bonus::Bonus() : Constructor called \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Destructor
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Bullet::~Bullet() {
//    LogInfo(BIT_BONUS, "Bonus::~Bonus() : Destructor called \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Load Bullet
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Bullet::LoadBulletImage(sf::RenderWindow &window) {
    TileMap::PrintControl_s printControl;

    printControl.imgXIdx = 7; /// Image of bullet initem images
    printControl.imgYIdx = 9 << BLOCK_SIZE_BIT;
    printControl.imgType = TileMap::ITEM;
    printControl.tileSize = m_TileVector;
    printControl.xTileOffset = BLOCK_SIZE_BIT;
    
    if (AIR == m_State) {
        if (IsDownCollision()) {
            SetState(JUMPING);
        }
        else {
            m_Position.X = (RIGHT == m_Direction) ? m_Position.X + gItemMoveFactor : m_Position.X - gItemMoveFactor;
            m_Position.Y += gItemMoveFactor;
            
            m_BulletX += gItemMoveFactor;
        }
    }
    else if (JUMPING == m_State) {
        if (m_BulletY > 8.0f) {
            m_BulletY = 0.0f;
            SetState(AIR);
        }
        else {
            m_Position.X = (RIGHT == m_Direction) ? m_Position.X + gItemMoveFactor : m_Position.X - gItemMoveFactor;
            m_Position.Y -= gItemMoveFactor;
            
            m_BulletY += gItemMoveFactor;
            m_BulletX += gItemMoveFactor;
        }
    }

    if (IsSideCollision()) { /// with the obstacles
        printControl.imgYIdx = 11 << BLOCK_SIZE_BIT;
        m_ExplosionCounter++;
    }

    if ((m_BulletX > 120) || (m_ExplosionCounter > gExplosionTime)) {
        SetState(DYING);
    }
    
    printControl.position = m_Position;

    m_Map.Load(printControl);
    window.draw(m_Map);

    return EXIT_SUCCESS;
}

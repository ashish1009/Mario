#include "Bullet.h"
#include "Logger.h"

const short gExplosionTime = 3;

Bullet::Bullet()
: m_TileVector(BLOCK_SIZE, BLOCK_SIZE) , m_BulletY(0), m_BulletX(0), m_ExplosionCounter(0) {
//    LogInfo(BIT_BONUS, "Bonus::Bonus() : Constructor called \n");
}

Bullet::~Bullet() {
//    LogInfo(BIT_BONUS, "Bonus::~Bonus() : Destructor called \n");
}

int Bullet::LoadItemImage(sf::RenderWindow &winMario, short frameX) {
    TileMap::PrintControl_s printControl;

    printControl.tileSet = ResourcePath + BLOCK_IMG_PATH;
    printControl.tile = 0;
    printControl.bInverted = false;
    printControl.tileSize = m_TileVector;
    printControl.imgY = Obstacle::FIRE_BULLET << BLOCK_SIZE_BIT;
    
    if (AIR == m_State) {
        if (IsDownCollision(frameX)) {
            SetState(JUMPING);
        }
        else {
            SetPosition(m_Position.X + 1, m_Position.Y + 1);
            m_BulletX++;
        }
    }
    else if (JUMPING == m_State) {
        if (m_BulletY > 8) {
            m_BulletY = 0;
            SetState(AIR);
        }
        else {
            SetPosition(m_Position.X + 1, m_Position.Y - 1);
            m_BulletY++;
            m_BulletX++;
        }
    }
    
    if (IsSideCollision(frameX, BLOCK_SIZE, m_Position.X + BLOCK_SIZE - (gExplosionTime - m_ExplosionCounter))) {
        printControl.tile = 1;
        m_ExplosionCounter++;
    }
    
    if ((m_BulletX > 120) || (m_ExplosionCounter > (gExplosionTime - 1))) {
        SetState(DYING);
    }
    printControl.position = m_Position;
    if (!m_Map.Load(printControl)) {
        LogError (BIT_BLOCK, "Block::LoadBlockImage() : Can Not Load Block Image \n");
        return EXIT_FAILURE;
    }
    
    winMario.draw(m_Map);
    return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Detect the collision with player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Bullet::IsPlayerCollision() {
    return false;
}

bool Bullet::IsDownCollision (const int frameX) {
    Obstacle *pObstacle = Obstacle::GetInstance();
    bool bIsLeftDownCollision = pObstacle->GetIsObstacleAt (m_Position.Y - BLOCK_SIZE_BIT, gPixelToBeLandedL + m_Position.X + frameX);
    bool bIsRightDownCollision = pObstacle->GetIsObstacleAt (m_Position.Y - BLOCK_SIZE_BIT , gPixelToBeLandedR + m_Position.X + frameX);
                            
    return (bIsLeftDownCollision || bIsRightDownCollision);
}

bool Bullet::IsJumpCollision (const int frameX) {
    return false;
}

bool Bullet::IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) {
    Obstacle *pObstacle = Obstacle::GetInstance();
    bool bIsSideUpCollision = pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX - BLOCK_SIZE_BIT);
    bool bIsSideDownCollision = pObstacle->GetIsObstacleAt(m_Position.Y - gPixelToColloidD, xPixelOfPlayer + frameX - BLOCK_SIZE_BIT);
    
    return (bIsSideUpCollision || bIsSideDownCollision);
}

#include "Enemy.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Constructor of Player Class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enemy::Enemy() {
//    LogInfo(BIT_PLAYER, "Player::Player(), Player Constructor called !! \n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Destructor of Player Class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enemy::~Enemy() {
//    LogInfo(BIT_PLAYER, "Player::~Player(), Player Destructor called !! \n");
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Load Player Image using Tile Map Classs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Enemy::LoadPlayerImage(sf::RenderWindow &winMario) {
//    TileMap::PrintControl_s printControl;
//
//    if (GROUND == m_State) {
//        if (((RUNNING != m_PrevState)) || (PlayerImgIdx::MID_INC == m_PlayerImgIdx)) {
//            m_PlayerImgIdx = PlayerImgIdx::STAND;
//        }
//    }
//    else {
//        if (SHOOTING == m_State) {
//            m_PlayerImgIdx = PlayerImgIdx::MID_INC + 1;
//            if (m_bShotFire) {
//                SetState(AIR);
//            }
//        }
//    }
//
//    ///  Get the Y Image Index for Player (each index contain both Small and Big Player)
//    const int ImgPosY = m_Ability * (PLAYER_HEIGHT_BIG + PLAYER_HEIGHT_SMALL);
//
//    printControl.tileSet = ResourcePath + PLAYER_IMG_PATH;
//    printControl.position = m_Position;
//    printControl.imgY = (SMALL == m_Size) ? ImgPosY + PLAYER_HEIGHT_BIG : ImgPosY;
//    printControl.tile = m_PlayerImgIdx;
//    printControl.tileSize = m_TileVector;
//    printControl.bInverted = (LEFT == m_Direction) ? true : false;
//
//    if (!m_Map.Load(printControl)) {
//        LogError (BIT_PLAYER, "Player::LoadPlayerImage() : Can not Load Image using TileMap \n");
//        return EXIT_FAILURE;
//    }
//
//    m_PlayerView.reset(sf::FloatRect(0.f, 0.f, WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT));
//    winMario.setView(m_PlayerView);
//    winMario.draw(m_Map);
//    return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the landing collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Enemy::IsDownCollision (const int frameX) {
//    bool bIsLeftDownCollision = m_pObstacle->GetIsObstacleAt (m_Position.Y, gPixelToBeLandedL + m_Position.X + frameX);
//    bool bIsRightDownCollision = m_pObstacle->GetIsObstacleAt (m_Position.Y, gPixelToBeLandedR + m_Position.X + frameX);
//
//    if (bIsLeftDownCollision || bIsRightDownCollision) {
//        return true;
//    }
//
//    /// Shift the plyer if few pixels are left for landing and those pixels are not enough for landing
//    else if ((m_pObstacle->GetIsObstacleAt (m_Position.Y , gPixelToBeLandedL - 2 + m_Position.X + frameX)) && (!bIsRightDownCollision)) {
//        SetPosition (m_Position.X + gPixelToBeLandedL, m_Position.Y);
//        SetState(AIR);
//        return false;
//    }
//
//    /// Shift the plyer if few pixels are left for landing and those pixels are not enough for landing
//    else if ((m_pObstacle->GetIsObstacleAt (m_Position.Y , gPixelToBeLandedR + 2 + m_Position.X + frameX)) && (!bIsLeftDownCollision)) {
//        SetPosition (m_Position.X - gPixelToBeLandedL, m_Position.Y);
//        SetState(AIR);
//        return false;
//    }
//    else {
        return  false;
//    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the Jumping collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Enemy::IsJumpCollision (const int frameX) {
//    bool bIsLeftUpCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - m_PlayerHeight, gPixelToBeUpColloidedL + m_Position.X + frameX);
//    bool bIsRightUpCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - m_PlayerHeight, gPixelToBeUpColloidedR + m_Position.X + frameX);
//
//    /// Set the block as popped so that it may behave according to blocs's behaviour e.g Coin, Mushroom or none
//    if (bIsLeftUpCollision || bIsRightUpCollision) {
//        m_pObstacle->SetPoppedBlock (m_Position.Y - m_PlayerHeight, m_Position.X + frameX + (PLAYER_WIDTH >> 1));
//        return true;
//    }
//
//    /// Shift the plyer to Right if few pixels are left for up Collision and those pixels are not enough for Up Left Collision
//    else if (m_pObstacle->GetIsObstacleAt (m_Position.Y - m_PlayerHeight, gPixelToBeUpColloidedL + m_Position.X - 2 + frameX) && (!bIsRightUpCollision)) {
//        SetPosition (m_Position.X + gPixelToBeUpColloidedL, m_Position.Y);
//        return false;
//    }
//
//    /// Shift the plyer to Left if few pixels are left for up Collision and those pixels are not enough for Up right Collision
//    else if (m_pObstacle->GetIsObstacleAt (m_Position.Y - m_PlayerHeight, gPixelToBeUpColloidedR + m_Position.X + 2 + frameX) && (!bIsLeftUpCollision)) {
//        SetPosition (m_Position.X - gPixelToBeUpColloidedL, m_Position.Y);
//        return false;
//    }
//
//    else {
        return false;
//    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the Side collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Enemy::IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) {
//    const int midPixel = (pixelToColloidU + gPixelToColloidD) >> 1;
//
//    bool bIsSideUpCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX);
//    bool bIsSideDownCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - gPixelToColloidD, xPixelOfPlayer + frameX);
//    bool bIsSideMidCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - midPixel, xPixelOfPlayer + frameX);
//
//    if (bIsSideUpCollision || bIsSideDownCollision || bIsSideMidCollision) {
//        return true;
//    }
//
//    /// Shift the plyer Up if few pixels are Down for landing and those pixels are not enough for for side collision
//    else if (m_pObstacle->GetIsObstacleAt (m_Position.Y - gPixelToColloidD + 2, xPixelOfPlayer + frameX) && (!bIsSideDownCollision)) {
//        SetPosition(m_Position.X + gPixelToBeLandedL, m_Position.Y - gPixelToColloidD - 3);
//        return false;
//    }
//
//    else {
        return false;
//    }
}

bool Enemy::IsPlayerCollision() {
    return false;
}

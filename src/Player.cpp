#include "Player.h"
#include "Logger.h"

const short PLAYER_START_X = 40;
const short FRAME_MOVE_START_PLAYER_X = 80;     /// Player Position after which background will move left
const short PLAYER_POS_X_STOP = 112;            /// Player Position after witch only Background will move
const short MAX_FRAME_MOVE = 3130;              /// Frame can max move to
const short MAX_JUMP_HEIGHT = 64;

Player *Player::m_pInstance = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Constructor of Player Class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player::Player()
: m_TileVector(PLAYER_WIDTH, PLAYER_HEIGHT_SMALL), m_PlayerImgIdx(PlayerImgIdx::STAND),
    m_PlayerMoveIdx(0), m_JumpFactor(0) {
    SetPosition(PLAYER_START_X, 150);
//    SetSize(BIG);
//    m_TileVector = {PLAYER_WIDTH, PLAYER_HEIGHT_BIG};

    LogInfo(BIT_PLAYER, "Player::Player(), Player Constructor called !! \n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Destructor of Player Class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player::~Player() {
    LogInfo(BIT_PLAYER, "Player::~Player(), Player Destructor called !! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Create new Instance for Player. If alredy created then return the older Pointer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player *Player::GetInstance() {
    if (nullptr == m_pInstance) {
        LogInfo(BIT_PLAYER, "Player::GetInstance(), Creating Player Instance() \n");
        m_pInstance = new Player();
    }
    return m_pInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : delete the created Instance
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::ReleaseInstance() {
    if (nullptr != m_pInstance) {
        LogInfo(BIT_PLAYER, "Player::ReleaseInstance(), Deleting Player Instance() \n");
        delete m_pInstance;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Load Player Image using Tile Map Classs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Player::LoadPlayerImage(sf::RenderWindow &winMario) {
    TileMap::PrintControl_s printControl;
    
    ///  Get the Y Image Index for Player (each index contain both Small and Big Player)
    const int ImgPosY = m_Ability * (PLAYER_HEIGHT_BIG + PLAYER_HEIGHT_SMALL);
    
    printControl.tileSet = ResourcePath + PLAYER_IMG_PATH;
    printControl.position = m_Position;
    printControl.imgY = (SMALL == m_Size) ? ImgPosY + PLAYER_HEIGHT_BIG : ImgPosY;
    printControl.tile = m_PlayerImgIdx;
    printControl.tileSize = m_TileVector;
    printControl.bInverted = (LEFT == m_Direction) ? true : false;

    if (!m_Map.Load(printControl)) {
        LogError (BIT_PLAYER, "Player::LoadPlayerImage() : Can not Load Image using TileMap \n");
        return EXIT_FAILURE;
    }

    m_PlayerView.reset(sf::FloatRect(0.f, 0.f, WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT));
    winMario.setView(m_PlayerView);
    winMario.draw(m_Map);
    return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Check state of Player
///          if AIR then Free fall if jumping then jump
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::CheckPlayerState(const int frameX) {
//    if (JUMPING == m_State) {
//        if (MAX_JUMP_HEIGHT < m_JumpFactor) {
//            SetState(AIR);
//        }
//        else {
//            for (int i = 0; i < m_Speed; i++) {
//                if (!IsCollision()) {
//                    m_JumpFactor++;
//                    SetPosition(m_Position.X, m_Position.Y - 1);
//                }
//                else {
//                    SetState(AIR);
//                    break;
//                }
//            }
//        }
//    }
//    else
    if (AIR == m_State) {
        const int freeFallSpeed = 1;
        for (int i = 0; i < freeFallSpeed; i++) {
            if (!IsCollision(frameX)) {
                SetPosition(m_Position.X, m_Position.Y + 1);
            }
            else {
                /// Land Player
                SetState(GROUND);
                break;
            }
        }
    }
    else {
        SetState(AIR);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Lands the player if collision
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::LandPlayer() {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Checks the collison of Player according to its state and moving direction
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Player::IsCollision(const int frameX) {
    Obstacle *pObstacle = Obstacle::GetInstance();
    short collisionPixel = 0;
    
    const int numPixelForCollision = 6;
    
    switch (m_State) {
        case AIR:
        {
            const int pixelToBeLandedL = 3;
            const int pixelToBeLandedR = 13;
            if ((pObstacle->GetIsObstacleAt(m_Position.Y, pixelToBeLandedL + m_Position.X + frameX)) ||
                (pObstacle->GetIsObstacleAt(m_Position.Y, pixelToBeLandedR + m_Position.X + frameX))) {

                return true;
            }
        }
        break;
            
        case GROUND:
            {
                
                const int playerHeight = (BIG == m_Size) ? PLAYER_HEIGHT_BIG : PLAYER_HEIGHT_SMALL;
                const int pixelToColloidD = 3;
                const int pixelToColloidU = playerHeight - pixelToColloidD;
                const int xPixelOfPlayer = ((RIGHT == m_Direction) ? (m_Position.X + PLAYER_WIDTH + 1) : (m_Position.X - 1));
                
                if ((pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX)) ||
                    (pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidD, xPixelOfPlayer + frameX))) {

                    return true;
                }
            }
            break;
            
        default:
            break;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Move the Player also changes the m_FramePositionX of Mario clsss
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::MoveRight(int &frameX, sf::RenderWindow &winMario) {
//    Obstacle *pObstacle = Obstacle::GetInstance();
    SetDirection(RIGHT);
    
    if (m_Speed < 4) {
        m_Speed += 1;
    }

//    const int playerHeight = (BIG == m_Size) ? PLAYER_HEIGHT_BIG : PLAYER_HEIGHT_SMALL;
//    const int pixelToColloidD = 3;
//    const int pixelToColloidU = playerHeight - pixelToColloidD;
//    const int xPixelOfPlayer = ((RIGHT == m_Direction) ? (m_Position.X + PLAYER_WIDTH + 1) : (m_Position.X - 1));

    for (int i = 0; i < m_Speed; i++) {
        if (GROUND == m_State) {
            SetPlayerImageIdx(PlayerImgIdx::RUN[m_PlayerMoveIdx++]);
            m_PlayerMoveIdx %= PlayerImgIdx::RUN_IDX_ARR_SIZE;
        }

//        if (!((pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX)) ||
//            (pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidD, xPixelOfPlayer + frameX)))) {
        if (!IsCollision(frameX)) {
            if (LEFT != m_Direction) {
//                    if (PLAYER_POS_X_STOP > m_Position.X) {
//                        SetPosition(m_Position.X + 1, m_Position.Y);
//                    }
                if (/*(FRAME_MOVE_START_PLAYER_X < m_Position.X) && */(MAX_FRAME_MOVE > frameX)) {
                    frameX += 1;
                }
            }
            else {
                SetDirection(RIGHT);
            }
            if (EXIT_FAILURE == LoadPlayerImage(winMario)) {
                LogError(BIT_MARIO, " Player::MoveRigh() : Can Not Load Player Image \n");
                winMario.close();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Move the Player also changes the m_FramePositionX of Mario clsss
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::MoveLeft(int &frameX, sf::RenderWindow &winMario) {
    SetDirection(LEFT);
    
    if (GROUND == m_State) {
        SetPlayerImageIdx(PlayerImgIdx::RUN[m_PlayerMoveIdx++]);
        m_PlayerMoveIdx %= PlayerImgIdx::RUN_IDX_ARR_SIZE;
    }
    
    if (m_Speed < 4) {
        m_Speed += 1;
    }
    
//            else if (LEFT == direction) {
//                if (RIGHT == m_Direction) {
//                    SetDirection(LEFT);
//                }
//                else {
//                    if (0 <= m_Position.X) {
//                        SetPosition(m_Position.X - 1, m_Position.Y);
//                    }
//                }
//            }

}

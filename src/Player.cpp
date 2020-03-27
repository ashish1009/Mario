#include "Player.h"
#include "Logger.h"

const int PLAYER_START_X = 40;
const int FRAME_MOVE_START_PLAYER_X = 80; ///Player Position after which background will move left
const int PLAYER_POS_X_STOP = 112; /// Player Position after witch only Background will move

Player *Player::m_pInstance = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Constructor of Player Class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player::Player()
: m_TileVector(PLAYER_WIDTH, PLAYER_HEIGHT_SMALL), m_PlayerImgIdx(PlayerImgIdx::STAND) {
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
/// Brief      : if Player State is AIR then Free fall the player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::LandPlayer() {
    if (AIR == m_State) {
        for (int i = 0; i < m_Speed; i++) {
            if (!IsCollision()) {
                SetPosition(m_Position.X, m_Position.Y + 1);
            }
            else {
                SetState(GROUND);
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Checks the collison of Player according to its state and moving direction
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Player::IsCollision() {
    Obstacle *pObstacle = Obstacle::GetInstance();
    short collisionPixel = 0;
    
    const int numPixelForCollision = 6;
    const int playerHeight = (Entity::BIG == m_Size) ? PLAYER_HEIGHT_BIG : PLAYER_HEIGHT_SMALL;
    
    switch (m_State) {
        case AIR:
            for (int i = 0; i < PLAYER_WIDTH; i++) {
                if (pObstacle->GetIsObstacleAt(m_Position.Y, i + m_Position.X)) {
                    collisionPixel++;
                    
                    if (numPixelForCollision == collisionPixel) {
                        return true;
                    }
                }
            }
            break;
            
        case GROUND:
            for (int i = 0; i < playerHeight; i++) {
                const int xPixelOfPlayer = ((RIGHT == m_Direction) ? (m_Position.X + PLAYER_WIDTH + 1) : (m_Position.X - 1));
                
                if (pObstacle->GetIsObstacleAt(m_Position.Y - i, xPixelOfPlayer)) {
                    collisionPixel++;
                    
                    if (numPixelForCollision == collisionPixel) {
                        return true;
                    }
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
void Player::Move(Entity::Direction_e direction, int &frameX) {
    SetDirection(direction);

    if (GROUND == m_State) {
        SetPlayerImageIdx(PlayerImgIdx::RUN[m_PlayerMoveIdx++]);
        m_PlayerMoveIdx %= PlayerImgIdx::RUN_IDX_ARR_SIZE;
    }

    for (int i = 0; i < m_Speed; i++) {
        if (!IsCollision()) {
            if (RIGHT == direction) {
                if (LEFT == m_Direction){
                    SetDirection(RIGHT);
                }
                else {
                    if (PLAYER_POS_X_STOP > m_Position.X) {
                        SetPosition(m_Position.X + 1, m_Position.Y);
                    }
                    if (FRAME_MOVE_START_PLAYER_X < m_Position.X) {
                        frameX += 8;
                        /// Remove First column pixels after moving that frame right
                        Obstacle::GetInstance()-> PopFirstColumnPixels(); ///As Obstacle is Singleton class no need to make any object
                        
                        /// As One column is removed we need to add another at end;
                        Obstacle::GetInstance()->PushLastColumnPixels();
                    }
                }
            }
            else if (LEFT == direction) {
                if (RIGHT == m_Direction) {
                    SetDirection(LEFT);
                }
                else {
                    if (0 <= m_Position.X) {
                        SetPosition(m_Position.X - 1, m_Position.Y);
                    }
                }
            }
            else {
                LogError(BIT_PLAYER, "Player::Move() : Player Directio is Invalid %d \n", m_Direction);
            }
        }
        else {
            break;
        }
    }
}

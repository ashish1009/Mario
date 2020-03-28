#include "Player.h"
#include "Logger.h"

const short PLAYER_START_X = 40;
const short FRAME_MOVE_START_PLAYER_X = 80;     /// Player Position after which background will move left
const short PLAYER_POS_X_STOP = 112;            /// Player Position after witch only Background will move
const short MAX_FRAME_MOVE = 3130;              /// Frame can max move to
const short MAX_JUMP_HEIGHT = 64;

const short gPixelToColloidD = 3;               /// First pixel from Down to be collided : can be changed

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

    m_pObstacle = Obstacle::GetInstance();
    LogInfo(BIT_PLAYER, "Player::Player(), Player Constructor called !! \n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Destructor of Player Class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player::~Player() {
    m_pObstacle->ReleaseInstance();
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
        m_pInstance = nullptr;
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
/// Brief      : Check state of Playe(     if AIR then Free fall if jumping then jump
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::CheckPlayerState(const int frameX, sf::RenderWindow &winMario) {
    Obstacle *pObstacle = Obstacle::GetInstance();
    const int pixelToBeColloidedL = 3;
    const int pixelToBeColloidedR = 13;
    
    if (JUMPING == m_State) {
        Jump();
    }
    else if (AIR == m_State) {
        if (IsDownCollision(frameX)) {
            /// Land Player
            SetState(GROUND);
            ResetJumpFactor();
        }
        else {
            SetPosition(m_Position.X, m_Position.Y + 1);
        }
    }
    else {
        SetState(AIR);
    }
    
    if (RUNNING != m_State) {
        if (EXIT_FAILURE == LoadPlayerImage(winMario)) {
            LogError(BIT_MARIO, " Player::CheckPlayerStatus() : Can Not Load Player Image \n");
            winMario.close();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Jump Player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Jump() {
    SetPlayerImageIdx(PlayerImgIdx::JUMP);
        
    if (MAX_JUMP_HEIGHT < m_JumpFactor) {
        SetState(AIR);
    }
    else {
        if (1) {
            m_JumpFactor++;
            SetPosition(m_Position.X, m_Position.Y - 1);
        }
        else {
            SetState(AIR);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Move the Player also changes the m_FramePositionX of Mario clsss
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Move(Entity::Direction_e direction, int &frameX, sf::RenderWindow &winMario) {
    SetDirection(direction);
    Obstacle *pObstacle = Obstacle::GetInstance();

    const int playerHeight = (BIG == m_Size) ? PLAYER_HEIGHT_BIG : PLAYER_HEIGHT_SMALL;
    const int pixelToColloidU = playerHeight - gPixelToColloidD;                         /// First pixel from last to be collided : can be changed
    const int xPixelOfPlayer = (RIGHT == m_Direction) ? (m_Position.X + PLAYER_WIDTH + 1) : (m_Position.X - 1);
    
    if (GROUND == m_State) {
        SetState(RUNNING);
    }
    
    if (MAX_SPEED > m_Speed) {
        m_Speed ++;
    }
    
    for (int i = 0; i < m_Speed; i++) {
        if (RUNNING == m_State) {
            SetPlayerImageIdx(PlayerImgIdx::RUN[m_PlayerMoveIdx]);
            m_PlayerMoveIdx++;
            m_PlayerMoveIdx %= PlayerImgIdx::RUN_IDX_ARR_SIZE;
        }
        
        else if (JUMPING == m_State) {
            SetSpeed(Entity::DEFAULT_SPEED);
            Jump();
        }
        
        if (m_Direction == RIGHT) {
            if (!((pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX)) ||
                (pObstacle->GetIsObstacleAt(m_Position.Y - gPixelToColloidD, xPixelOfPlayer + frameX)))) {
                
                if (PLAYER_POS_X_STOP > m_Position.X) {
                    SetPosition(m_Position.X + 1, m_Position.Y);
                }
                else {
                    if (MAX_FRAME_MOVE > frameX) {
                        frameX++;
                    }
                }
                
                if (EXIT_FAILURE == LoadPlayerImage(winMario)) {
                    LogError(BIT_MARIO, " Player::MoveRigh() : Can Not Load Player Image \n");
                    winMario.close();
                }
            }
        }
        else {
            if (!((pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX)) ||
                (pObstacle->GetIsObstacleAt(m_Position.Y - gPixelToColloidD, xPixelOfPlayer + frameX)))) {
            
                if (0 <= m_Position.X) {
                    SetPosition(m_Position.X - 1, m_Position.Y);
                }
            }
        }
    }
}

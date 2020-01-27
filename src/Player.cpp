#include "Player.h"
#include "Logger.h"

#define PLAYER_X_SCALE_FACTOR 1.5
#define PLAYER_Y_SCALE_FACTOR 1

Player *Player::m_pInstance = nullptr;

Player::Player()
    : m_Vector(PLAYER_WIDTH, PLAYER_HEIGHT_SMALL), m_PrintSize(PLAYER_WIDTH * PLAYER_X_SCALE_FACTOR, PLAYER_HEIGHT_SMALL << PLAYER_Y_SCALE_FACTOR) ,m_PlayerImg(0)
{
//    IncSize();
//    SetPosition(PLAYER_BASE_X, PLAYER_BASE_Y - 50);
    SetPosition(0, 0);
    m_pObstacle = Obstacle::GetInstance();
    LogDebug(BIT_PLAYER, "Player::Player(), Player Constructor called !! \n");
}

Player::~Player()
{
    LogDebug(BIT_PLAYER, "Player::~Player(), Player Destructor called !! \n");
}

void Player::Init()
{
    SetPosition(PLAYER_BASE_X, PLAYER_BASE_Y - 50);
    m_Vector = {PLAYER_WIDTH, PLAYER_HEIGHT_SMALL};
    m_PrintSize = {PLAYER_WIDTH * PLAYER_X_SCALE_FACTOR, PLAYER_HEIGHT_SMALL << PLAYER_Y_SCALE_FACTOR};
    m_PlayerImg = 0;
}

Player *Player::GetInstance()
{
    if (nullptr == m_pInstance)
    {
        LogInfo(BIT_PLAYER, "Player::GetInstance(), Creating Player Instance() \n");
        m_pInstance = new Player();
    }
    return m_pInstance;
}

void Player::ReleaseInstance()
{
    if (nullptr != m_pInstance)
    {
        LogInfo(BIT_PLAYER, "Player::ReleaseInstance(), Deleting Player Instance() \n");
        delete m_pInstance;
    }
}

int Player::LoadPlayerImage()
{
    TileMap::PrintControl_s PrintControl;
    
    const unsigned int ImgSize = m_Ability * (PLAYER_HEIGHT_BIG + PLAYER_HEIGHT_SMALL);
    
    PrintControl.TileSet = ResourcePath + PLAYER_IMG_PATH;
    PrintControl.Position = m_Position;
    PrintControl.ImgY = (SMALL == m_Size) ? ImgSize + PLAYER_HEIGHT_BIG : ImgSize;
    PrintControl.Tile = m_PlayerImg;
    PrintControl.TileSize = m_Vector;
    PrintControl.PrintSize = m_PrintSize;
    PrintControl.bInverted = (LEFT == m_Direction) ? true : false;
    PrintControl.bObstacle = false;

    if (!m_Map.Load(PrintControl))
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

void Player::DrawPlayer(sf::RenderWindow &m_WinMario) const
{
    sf::View view2;
    m_WinMario.setView(view2);
    m_WinMario.draw(m_Map);
}

void Player::Move(float &FrameX, const Player::Direction_e Direction)
{
#define LEVEL_1_LIMIT 2980
#define LEVEL_1_PLAYER_LIMIT 290
    
    static short PlayerMoveIdx = 0;
    PlayerMoveIdx++;
    
    PlayerMoveIdx = PlayerMoveIdx % PlayerImgIdx::RUN_IDX_ARR_SIZE;
    SetPlayerImg(PlayerImgIdx::RUN[PlayerMoveIdx]);

    SetDirection(Direction);
    
    if ((!IsCollision()) && (DYING != m_Behaviour))
    {
        if (RIGHT == Direction)
        {
            if ((LEVEL_1_LIMIT > FrameX) && (PLAYER_BASE_X <= m_Position.X))
            {
                if (100 >= m_Position.X)
                {
                    SetPosition(m_Position.X + m_Speed, m_Position.Y);
                }
                else
                {
                    FrameX += m_Speed;
                    for(short i = 0; i < m_Speed; i++)
                    {
                        m_pObstacle->PopFirstColumnPixels(); //TODO push
                    }
                }
            }
            else if(LEVEL_1_PLAYER_LIMIT >= m_Position.X)
            {
                SetPosition(m_Position.X + m_Speed, m_Position.Y);
            }
        }
        else if (LEFT == Direction)
        {
            if(0 < m_Position.X)
            {
                SetPosition(m_Position.X - m_Speed, m_Position.Y);
            }
        }
        SetBehaviour(AIR);
    }
}

void Player::IncSize()
{
    SetSize(BIG);
    m_Vector = {PLAYER_WIDTH, PLAYER_HEIGHT_BIG};
    m_PrintSize = {PLAYER_WIDTH * PLAYER_X_SCALE_FACTOR, PLAYER_HEIGHT_SMALL << PLAYER_Y_SCALE_FACTOR};
}

void Player::SetNewPosition()
{
    if(((AIR == m_Behaviour) || (DYING == m_Behaviour)) && (!IsCollision()))
    {
        const short PlayerHead = WIN_HEIGHT + ((SMALL == m_Size) ? PLAYER_HEIGHT_SMALL : PLAYER_HEIGHT_BIG);
        if(PlayerHead > m_Position.Y)
        {
            SetPosition(m_Position.X, m_Position.Y + 1);
            if(PLAYER_BASE_Y + 1 == m_Position.Y )
            {
                SetBehaviour(DYING);
                SetSize(SMALL);
                m_Vector = {PLAYER_WIDTH, PLAYER_HEIGHT_SMALL};
            }
        }
        else
        {
//            PlayerDead();
            Init();
        }
    }
}

bool Player::IsCollision()
{
//    LogDebug(BIT_PLAYER, "isCollision(), x %f, Y %f, %d, \n", m_Position.X, m_Position.Y, m_Obstacle.IsEmptyFramePixel(m_Position.X, m_Position.Y));
    switch (m_Behaviour)
    {
        case AIR:
        case DYING:
//            LogDebug(BIT_PLAYER, "Collision() AIR, X: %f, Y: %f, colli", m_Position.X, m_Position.Y);
                              
            for (int i = m_Position.X; i < m_Position.X + PLAYER_WIDTH; i++)
            {
//                printf(" %d ", m_pObstacle->IsEmptyFramePixel(i, m_Position.Y));
                if(m_pObstacle->IsEmptyFramePixel(i, m_Position.Y))
                {
                    m_Behaviour = GROUND;
                    return true;
                }
            }
//            LogDebug(BIT_PLAYER, "\n");
            break;

        case GROUND:
            if (RIGHT == m_Direction)
            {
                const unsigned short PlayerHeight = (SMALL == m_Size) ? PLAYER_HEIGHT_SMALL : PLAYER_HEIGHT_BIG;
                short X = (short)((PLAYER_WIDTH * PLAYER_X_SCALE_FACTOR) * WIN_WIDTH / 1000);

//                LogDebug(BIT_PLAYER, "Collision(), RIGHT X: %f, Y %f colll %d \n", m_Position.X + X, m_Position.Y, m_pObstacle->IsEmptyFramePixel(m_Position.X + X, m_Position.Y - 1));
                
                for (short i = m_Position.Y - PlayerHeight; i < m_Position.Y; i++)
                {
                    if(m_pObstacle->IsEmptyFramePixel(m_Position.X + X, i))
                        return true;
                }
            }
            break;

        default:
            break;
    }
    return false;
}

#include "Player.h"
#include "Logger.h"

#define PLAYER_X_SCALE_FACTOR 1.5
#define PLAYER_Y_SCALE_FACTOR 1

Player *Player::m_pInstance = nullptr;

Player::Player()
    : m_Vector(PLAYER_WIDTH, PLAYER_HEIGHT_SMALL), m_PrintSize(PLAYER_WIDTH * PLAYER_X_SCALE_FACTOR, PLAYER_HEIGHT_SMALL << PLAYER_Y_SCALE_FACTOR) ,m_PlayerImg(0), m_JumpHeight(DEFAULT_JUMP_HEIGHT)
{
//    IncSize();
    SetPosition(PLAYER_BASE_X, PLAYER_BASE_Y - 50);
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
    
    if(GROUND == m_Behaviour)
    {
        static short PlayerMoveIdx = 0;
        PlayerMoveIdx++;
        
        PlayerMoveIdx = PlayerMoveIdx % PlayerImgIdx::RUN_IDX_ARR_SIZE;
        SetPlayerImg(PlayerImgIdx::RUN[PlayerMoveIdx]);
    }
    
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
        if(JUMPING != m_Behaviour)
        {
            SetBehaviour(AIR);
        }
    }
}

void Player::IncSize()
{
    SetSize(BIG);
    m_Vector = {PLAYER_WIDTH, PLAYER_HEIGHT_BIG};
    m_PrintSize = {PLAYER_WIDTH * PLAYER_X_SCALE_FACTOR, PLAYER_HEIGHT_SMALL << PLAYER_Y_SCALE_FACTOR};
}

void Player::JumpPlayer()
{
//    LogDebug(BIT_PLAYER, "jumping(), y: %f, (PLAYER_BASE_Y - m_JumpHeight) %d \n", m_Position.Y, (PLAYER_BASE_Y - m_JumpHeight));
    if(!IsCollision())
    {
        SetPlayerImg(PlayerImgIdx::JUMP);
        if(m_Position.Y > (PLAYER_BASE_Y - m_JumpHeight))
        {
            SetPosition(m_Position.X, m_Position.Y - JUMP_FACTOR);
        }
        else if(m_Position.Y <= (PLAYER_BASE_Y - m_JumpHeight))
        {
            SetBehaviour(AIR);
        }
    }
}

void Player::SetNewPosition()
{
//    LogDebug(BIT_PLAYER,"State %d \n", m_Behaviour);
    if(((AIR == m_Behaviour) || (DYING == m_Behaviour)) && (!IsCollision()))
    {
        const short PlayerHead = WIN_HEIGHT + ((SMALL == m_Size) ? PLAYER_HEIGHT_SMALL : PLAYER_HEIGHT_BIG);
        if(PlayerHead > m_Position.Y)
        {
//            LogDebug(BIT_PLAYER, "Dec Height \n");
            SetPosition(m_Position.X, m_Position.Y + JUMP_FACTOR);
            if(PLAYER_BASE_Y + 1 <= m_Position.Y )
            {
                SetBehaviour(DYING);
                SetPosition(m_Position.X, PLAYER_BASE_Y);
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
#define DEBUG_PRINT_FLAG 0
#define PLAYER_COLLISION_MARGIN 0
    #define NUM_PIXEL_FOR_LANDING 5
    short NumPixelLanded = 0;
    const unsigned short PlayerHeight = (SMALL == m_Size) ? PLAYER_HEIGHT_SMALL : PLAYER_HEIGHT_BIG;
    
    short X = (short)((PLAYER_WIDTH * PLAYER_X_SCALE_FACTOR) * WIN_WIDTH / 1000);
    short Y = (short)((PlayerHeight << PLAYER_Y_SCALE_FACTOR) * WIN_HEIGHT / 1000);
    switch (m_Behaviour)
    {
        case AIR:
        case DYING:
            if(DEBUG_PRINT_FLAG)
                LogDebug(BIT_PLAYER, "Landing collision Chek %f ::", m_Position.Y);
            for (int i = m_Position.X + PLAYER_COLLISION_MARGIN; i < m_Position.X + X - PLAYER_COLLISION_MARGIN; i++)
            {
                if(DEBUG_PRINT_FLAG)
                    printf("%d(%d) ", m_pObstacle->IsEmptyFramePixel(i, m_Position.Y), i);
                if(m_pObstacle->IsEmptyFramePixel(i, m_Position.Y))
                {
                    NumPixelLanded++;
                    if(NUM_PIXEL_FOR_LANDING < NumPixelLanded)
                    {
                        LogInfo(BIT_PLAYER,"Landing Collision %d\n", NumPixelLanded);
                        m_Behaviour = GROUND;
                        return true;
                    }
                }
            }
            if(DEBUG_PRINT_FLAG)
                printf("\n");
            break;

        case GROUND:
        case JUMPING:
            if (RIGHT == m_Direction)
            {
                if(DEBUG_PRINT_FLAG)
                    LogDebug(BIT_PLAYER, "Righ collision Chek %f :: ", m_Position.X + X);
                for (short i = m_Position.Y - Y; i < m_Position.Y; i++)
                {
                    if(DEBUG_PRINT_FLAG)
                        printf("%d(%d) ", m_pObstacle->IsEmptyFramePixel(m_Position.X + X, i), i);
                    if(m_pObstacle->IsEmptyFramePixel(m_Position.X + X, i))
                    {
                        LogInfo(BIT_PLAYER,"Right Collision \n");
                        SetBehaviour(AIR);
                        return true;
                    }
                }
                if(DEBUG_PRINT_FLAG)
                    printf("\n");
            }
            else if (LEFT == m_Direction)
            {
                if(DEBUG_PRINT_FLAG)
                    LogDebug(BIT_PLAYER, "Left collision Chek %f ::", m_Position.X);
                for (short i = m_Position.Y - Y; i < m_Position.Y; i++)
                {
                    if(DEBUG_PRINT_FLAG)
                        printf("%d(%d) ", m_pObstacle->IsEmptyFramePixel(m_Position.X + X, i), i);
                    if(m_pObstacle->IsEmptyFramePixel(m_Position.X, i))
                    {
                        LogInfo(BIT_PLAYER,"Left Collision \n");
                        SetBehaviour(AIR);
                        return true;
                    }
                }
                if(DEBUG_PRINT_FLAG)
                    printf("\n");
            }
            {
                if(DEBUG_PRINT_FLAG)
                    LogDebug(BIT_PLAYER, "Jump collision Chek %f :: ", m_Position.Y -Y);
                for (int i = m_Position.X + PLAYER_COLLISION_MARGIN; i < m_Position.X + X - PLAYER_COLLISION_MARGIN; i++)
                {
                    if(DEBUG_PRINT_FLAG)
                        printf("%d(%d) ", m_pObstacle->IsEmptyFramePixel(i, m_Position.Y - Y), i);
                    if(m_pObstacle->IsEmptyFramePixel(i, m_Position.Y - Y))
                    {
                        LogInfo(BIT_PLAYER, "Jump Collision \n");
                        SetBehaviour(AIR);
                        return true;
                    }
                }
                if(DEBUG_PRINT_FLAG)
                    printf("\n");
            }
            break;
        
        default:
            break;
    }
    return false;
}

#include "Mario.h"
#include "Logger.h"

#define TEXT_SIZE 30
#define PRINT_Y_POS 20

Mario::Mario()
:     m_WinMario({WIN_WIDTH, WIN_HEIGHT}, "Super MArio"),
m_FramePosition(0) , m_Score(0), m_CoinCount(0), m_Level(1), m_Time(TIME_LIMIT)
{
    m_Texture.loadFromFile(ResourcePath + "graphics/level1.png");
    m_Sprite.setTexture(m_Texture);
    
    m_CoinScoreTexture.loadFromFile(ResourcePath + "graphics/Coin.png");
    m_CoinScoreSprite.setTexture(m_CoinScoreTexture);

    m_Font.loadFromFile(ResourcePath + "fonts/Mario.ttf");
    m_Text.setFont(m_Font);
    m_Text.setCharacterSize(TEXT_SIZE);

    m_pPlayer = Player::GetInstance();
    m_pObstacle = Obstacle::GetInstance();
    
    LogDebug(BIT_MARIO, "Mario::Mario(), Constructor called \n");
}

Mario::~Mario()
{
    m_pPlayer->ReleaseInstance();
    LogDebug(BIT_MARIO, "Mario::Mario(), Destructor called \n");
}

int Mario::PlayGame()
{
    LogDebug(BIT_MARIO, "Mario::PlayeGame(), Playing Game \n");
    while (m_WinMario.isOpen())
    {        
        ResetScreenAndPlayer();
        while (m_WinMario.pollEvent(m_MarioEvent))
        {
            switch (m_MarioEvent.type)
            {
            case sf::Event::Closed:
                LogWarning(BIT_MARIO, "Mario::PlayGame(), Game is Closed \n");
                m_WinMario.close();
                break;
                    
            case sf::Event::MouseMoved:
                m_Score = m_MarioEvent.mouseMove.x;
                m_CoinCount = m_MarioEvent.mouseMove.y;
                    
//                m_Level = m_pObstacle->m_bIsObstacle[m_MarioEvent.mouseMove.x][m_MarioEvent.mouseMove.y];
                break;
                    
            case sf::Event::KeyPressed:
                if(m_MarioEvent.key.code == sf::Keyboard::Right)
                {
                    MovePlayer(Entity::RIGHT);
                }
                else if(m_MarioEvent.key.code == sf::Keyboard::Left)
                {
                    MovePlayer(Entity::LEFT);
                }
                if(m_MarioEvent.key.code == sf::Keyboard::LSystem)
                {
                    m_pPlayer->SetBehaviour(Entity::AIR);
                }
                break;
                
            default:
                break;
            }
        }
        
        MoveBgAt();
        DrawPlayer();
        DrawBlock();
        PrintContent();
        
        m_WinMario.display();
    }
    return EXIT_SUCCESS;
}

void Mario::MoveBgAt()
{
    m_View.move(m_FramePosition, 0);
    m_WinMario.setView(m_View);
    m_WinMario.draw(m_Sprite);
}

void Mario::DrawPlayer()
{
//    if(Entity::DYING != m_pPlayer->GetBehaviour())
    {
        m_pPlayer->SetNewPosition();
        m_pPlayer->LoadPlayerImage();
        m_pPlayer->DrawPlayer(m_WinMario);
    }
}

void Mario::DrawBlock()
{
#define BLOCK_Y 200
#define BLOCK_X 200
    if(0 < (BLOCK_X - m_FramePosition))
    {
        Entity::Position_s Position;
        Position.SetPostion(BLOCK_X - m_FramePosition, BLOCK_Y);
        m_Block.LoadBlockImage(Position);
        m_Block.DrawBlock(m_WinMario);
    }
}

void Mario::PrintText(const char *PrintStuff, unsigned short Variable, const float Xposition)
{
#define PRINT_STRING_SIZE 20
    
    char *PrintString = new char[PRINT_STRING_SIZE];
    sprintf(PrintString, "%s %d", PrintStuff, Variable);
    m_Text.setPosition(Xposition, PRINT_Y_POS);
    m_Text.setString(PrintString);
    m_WinMario.draw(m_Text);
}

void Mario::PrintContent()
{
#define PRINT_POS_DIFF 250
#define SCORE_TEXT_POS 50
#define COIN_PRINT_POS 40
#define COIN_TEXT_POS SCORE_TEXT_POS + PRINT_POS_DIFF
#define LEVEL_TEXT_POS COIN_TEXT_POS + PRINT_POS_DIFF
#define TIME_TEXT_POS LEVEL_TEXT_POS + PRINT_POS_DIFF
   
    PrintText("MARIO \n", m_Score, SCORE_TEXT_POS);
    PrintText("\n  x", m_CoinCount, COIN_TEXT_POS);
    PrintText("WORLD \n1 - ", m_Level, LEVEL_TEXT_POS);
    PrintText("TIME \n", (unsigned short)(TIME_LIMIT - m_Time), TIME_TEXT_POS);
    
    m_CoinScoreSprite.setPosition(COIN_TEXT_POS, PRINT_Y_POS + COIN_PRINT_POS);
    m_WinMario.draw(m_CoinScoreSprite);
}

void Mario::ResetScreenAndPlayer()
{
    m_WinMario.clear();
    m_View.reset(sf::FloatRect(0.f, 0.f, MARIO_WIDTH, MARIO_HEIGHT));
    
    m_pPlayer->SetPlayerImg(PlayerImgIdx::STAND);
//    m_pPlayer->SetBehaviour(Entity::AIR);
        
    sf::Time time = m_Clock.getElapsedTime();
    m_Time = time.asSeconds();
}

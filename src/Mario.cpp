#include "Mario.h"
#include "Logger.h"
#include "Block.h"
#include <math.h>

#define TEXT_SIZE 30
#define PRINT_Y_POS 20

const int TIME_LIMIT = 400;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Constructor of Mario class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mario::Mario()
: m_WinMario({WIN_WIDTH, WIN_HEIGHT}, "Super MArio"),
m_pBonus(nullptr), m_FramePosition(0), m_Score(0), m_CoinCount(0), m_Level(1), m_Time(0) {
    m_MarioTexture.loadFromFile(ResourcePath + "graphics/level1.png");
    m_MarioSprite.setTexture(m_MarioTexture);
    
    m_CoinScoreTexture.loadFromFile(ResourcePath + "graphics/Coin.png");
    m_CoinScoreSprite.setTexture(m_CoinScoreTexture);
    
    m_Font.loadFromFile(ResourcePath + "fonts/Mario.ttf");
    m_Text.setFont(m_Font);
    m_Text.setCharacterSize(TEXT_SIZE);
    
    m_pPlayer = Player::GetInstance();
    m_pObstacle = Obstacle::GetInstance();
     
    LogInfo(BIT_MARIO, "Mario::Mario(), Constructor called \n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Destructor of Mario class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mario::~Mario() {
    m_pPlayer->ReleaseInstance();
    m_pObstacle->ReleaseInstance();
    
    LogInfo(BIT_MARIO, "Mario::~Mario(), Destructor called \n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Function that runs the GAME Loop
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::PlayGame() {
    LogInfo(BIT_MARIO, "Mario::PlayeGame(), Game is On... \n");
    
    m_FramePosition = 250;
    while (m_WinMario.isOpen()) {
        ResetScreen();
        SetTime();
        
        while (m_WinMario.pollEvent(m_MarioEvent)) {
            switch (m_MarioEvent.type) {
                case sf::Event::Closed:
                    LogWarning(BIT_MARIO, "Mario::PlayGame(), Game is Closed \n");
                    m_WinMario.close();
                    break;
                
                case sf::Event::KeyPressed:
                    if (m_MarioEvent.key.code == sf::Keyboard::LShift) {
                        if (Entity::AIR != m_pPlayer->GetState()) {
                            m_pPlayer->SetState(Entity::JUMPING);
                        }
                    }
                    else {
                        if (m_MarioEvent.key.code == sf::Keyboard::Right) {
                            m_pPlayer->Move(Entity::RIGHT, m_FramePosition, m_WinMario);
                        }
                        else if (m_MarioEvent.key.code == sf::Keyboard::Left) {
                            m_pPlayer->Move(Entity::LEFT, m_FramePosition, m_WinMario);
                        }
                    }
                    break;
                    
                case sf::Event::KeyReleased:
                    if ((m_MarioEvent.key.code == sf::Keyboard::Right) || (m_MarioEvent.key.code == sf::Keyboard::Left)) {
                        m_pPlayer->SetPlayerImageIdx(PlayerImgIdx::STAND);
                        m_pPlayer->RestePlayerMoveIdx();
                        m_pPlayer->SetSpeed(Entity::DEFAULT_SPEED);
//                        m_pPlayer->SetState(Entity::AIR);
                    }
                    break;
                                        
//                case sf::Event::MouseMoved:
//                    m_Score = (m_MarioEvent.mouseMove.x * WORLD_VIEW_WIDTH) / m_WinSize.x;
//                    m_CoinCount = (m_MarioEvent.mouseMove.y * WORLD_VIEW_HEIGHT) / m_WinSize.y;
//
//                    m_Level = m_pObstacle->GetIsObstacleAt(m_CoinCount, m_Score + m_FramePosition);
//                    m_Time = m_pPlayer->GetState();
//                     m_Time = m_pPlayer->GetSpeed();
//                    break;
                    
                default:
                    break;
            } /// switch (m_MarioEvent.type)
        } /// while (m_WinMario.pollEvent(m_MarioEvent))
        DrawView();
//        DrawPlayer();
        PrintContent();
        
        m_WinMario.display();
     } /// while (m_WinMario.isOpen())
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Reset the contents of Screen periodically
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Mario::ResetScreen() {
    m_WinSize = m_WinMario.getSize();
    m_WinMario.clear();
    m_MarioView.reset(sf::FloatRect(0.f, 0.f, WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : increase m_time per second
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Mario::SetTime() {
    sf::Time time = m_Clock.getElapsedTime();
    m_Time = time.asSeconds();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Move the background world to position m_FramePosition,  Change the View of the world
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::DrawView() {
    m_MarioView.move(m_FramePosition, 0);
    m_WinMario.setView(m_MarioView);
    m_WinMario.draw(m_MarioSprite);
    
    m_pPlayer->CheckPlayerState(m_FramePosition, m_WinMario);
    DrawBlocks();
    
    if (m_pBonus) {
        /// As block is popped the object is created then load the images
        m_pBonus->LoadBlockImage(m_WinMario, m_FramePosition, m_pBonus->getBlock());
        
        if (Entity::DYING == m_pBonus->GetState()) {
            /// if Bonus Gift is no more present in game then release the instance
            m_pBonus = Bonus::ReleaseInstance();
            m_CoinCount++;
            m_Score+=100;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Draw Block Image according to its state
///         m_WinMario get Updated
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::DrawBlocks() {
    const int cNumColView = WORLD_VIEW_WIDTH >> BLOCK_SIZE_BIT;     /// to divide by Block SIze = 16
    const int xOffset = m_FramePosition >> BLOCK_SIZE_BIT;          /// to divide by Block SIze = 16
    
    for (int i = 0; i < NUM_ROW; i++) {
        for (int j = 0; j <= cNumColView; j++) {
            Obstacle::ObstacleBlock_s *blockObst = m_pObstacle->GetBlockReference(i, j + xOffset);
            if (Obstacle::BRICK == blockObst->behaviour || Obstacle::BONUS == blockObst->behaviour) {
                
                /// Only Brick and Bonus Need to draw at run time As only these two will change at run time
                Block block;
                if (EXIT_FAILURE == block.LoadBlockImage(m_WinMario, m_FramePosition, i, j + xOffset, blockObst)) {
                    m_WinMario.close();
                }
                if (blockObst->bIsPopped && !blockObst->bIsEmpty) {
                    /// If Block is popped and it is not empty it will create the Bonus object for coin of Mushroom
                    m_pBonus = Bonus::CreateInstance();
                    m_pBonus->SetFramePos(m_FramePosition);         /// Store the current frame position, fo future use in loadBlockImage
                    m_pBonus->SetXPos(blockObst->xPos);
                    
                    if (m_pBonus) {
                        m_pBonus->SetBlock(blockObst);              /// Store the block type
                        if (!blockObst->bIsEmpty) {
                            /// Store the Initial position of Gift as 1 size up than block
                            m_pBonus->SetPosition(blockObst->xPos, blockObst->yPos - BLOCK_SIZE);
                        }
                    }
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Draw Player Image according to its state
///         m_WinMario get Updated
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::DrawPlayer() {
//    m_pPlayer->CheckPlayerState(m_FramePosition, m_WinMario);
//    if (EXIT_FAILURE == m_pPlayer->LoadPlayerImage(m_WinMario)) {
//        LogError(BIT_MARIO, " Mario::DrawPlayer() : Can Not Load Player Image \n");
//        m_WinMario.close();
//    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Reset The Mario View and print the contents at specific Position
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::PrintContent() {
#define PRINT_POS_DIFF 250
#define SCORE_TEXT_POS 50
#define COIN_PRINT_POS 40
#define COIN_TEXT_POS SCORE_TEXT_POS + PRINT_POS_DIFF
#define LEVEL_TEXT_POS COIN_TEXT_POS + PRINT_POS_DIFF
#define TIME_TEXT_POS LEVEL_TEXT_POS + PRINT_POS_DIFF
   
    sf::View textView;
    m_WinMario.setView(textView);

    PrintText("MARIO \n", m_Score, SCORE_TEXT_POS);
    PrintText("\n  x", m_CoinCount, COIN_TEXT_POS);
    PrintText("WORLD \n1 - ", m_Level, LEVEL_TEXT_POS);
    PrintText("TIME \n", (short)(TIME_LIMIT - m_Time), TIME_TEXT_POS);
    
    m_CoinScoreSprite.setPosition(COIN_TEXT_POS, PRINT_Y_POS + COIN_PRINT_POS);
    m_WinMario.draw(m_CoinScoreSprite);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Funtion to store the text in variable as string to print on window
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::PrintText(const char *printStuff, short variable, const float xPosition) {
    char *printString = new char[20];  // 20 is max String Size to be  printed
    sprintf(printString, "%s %d", printStuff, variable);
    m_Text.setPosition(xPosition, PRINT_Y_POS);
    m_Text.setString(printString);
    m_WinMario.draw(m_Text);
}

#include "Mario.h"
#include "Logger.h"
#include "Timer.h"
#include <algorithm>

const short gNumRowToDraw = 2;
const short gNumColView = VIEW_WIDTH >> BLOCK_SIZE_BIT;     /// to divide by Block SIze = 16
const std::array<short, gNumRowToDraw> gRowBlockArr {{gRow5ForBlock, gRow9ForBlock}};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mario::Mario()
: m_Score(0), m_CoinCoint(0), m_TimeLeft(MAX_TIME), m_FrameShiftX(0), m_pObstacle(nullptr), m_pPlayer(nullptr) {
    m_pObstacle = Obstacle::GetInstance();
    m_pPlayer = Player::GetInstance();
    LogInfo(BIT_MARIO, "Mario::Mario(), Constructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mario::~Mario() {
    m_pObstacle = Obstacle::ReleaseInstance();
    m_pPlayer = Player::ReleaseInstance();
    LogInfo(BIT_MARIO, "Mario::~Mario(), Destructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Play The Game
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::PlayGame() {
    LogInfo (BIT_MARIO, "Mario::PlayeGame(), Game is On... \n");
    
    auto ResetScreen = [&]() {
        m_Window.clear();
        m_View.reset(sf::FloatRect(0.0f, 0.0f, VIEW_WIDTH, VIEW_HEIGHT));
    };
    
    auto DrawView = [&]() {
        m_View.move(m_FrameShiftX, 0);
        m_Window.setView(m_View);
        m_Window.draw(m_Sprite);
    };
    
    if (!LoadRenderGraphics()) {
        return;
    }
    if (!m_Music.openFromFile(gResourcePath + "music/main_theme.ogg")) {
        LogError (BIT_MARIO, "Mario::PlayGame(), Can not load Mariio Theme Music ");
        return;
    }
    
//    m_FrameShiftX = 920;
//    StartScreen();
//    m_Music.play();
    
    m_Clock.restart();
    while (m_Window.isOpen()) {
        m_TimeLeft = MAX_TIME - m_Clock.getElapsedTime().asSeconds();
        
        ResetScreen();
        PolEvent();
        
        DrawView();
        DrawBlocks();
        DrawPlayer();
        DrawItems();
        DrawBullets();
        PrintContents(m_Score, m_CoinCoint, m_TimeLeft);
        
        m_Window.display();
    } /// while (m_WinMario.isOpen())
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw Bullets
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::DrawBullets () {
    auto it = m_lBullet.begin();
    for (; it != m_lBullet.end(); it++) {
        it->LoadBulletImage(m_Window);
        if (Entity::DYING == it->GetState()) {
            m_lBullet.erase(it);
            LogInfo(BIT_MARIO, "Mario::DrawItems(), Num Bullets %d \n", m_lItem.size());
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw Items and bricks
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::DrawItems() {
    auto it = m_lItem.begin();
    for (; it != m_lItem.end(); it++) {
        it->LoadItemImage(m_Window);
        if (Entity::DYING == it->GetState()) {
            if (COIN_BONUS == it->GetBlock()->m_Abilty) {
                m_Score += 100;
                m_CoinCoint ++;
            }
            if (MUSHROOM_BONUS == it->GetBlock()->m_Abilty) {
                m_Score += 1000;
                m_Sound.setBuffer(m_PowerUpSound);
                m_Sound.play();
            }
            
            m_lItem.erase(it);
            LogInfo(BIT_MARIO, "Mario::DrawItems(), Num Items %d \n", m_lItem.size());
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw blocks and bricks
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::DrawBlocks() {
    const float xOffset = static_cast<short>(m_FrameShiftX) >> BLOCK_SIZE_BIT;  /// frame offset in terms of block
    for (auto &row : gRowBlockArr) {
        for (int col = 0; col <= gNumColView; col++) {
            m_pObstacle->LoadBlockImage(m_Window, row, col + xOffset);
            
            Block *pBlock = m_pObstacle->GetBlock(row, col + xOffset);
            if ((pBlock->m_bIsPopped) && (1 == pBlock->m_UpPopped)){
                Item item;
                item.SetPosition(pBlock->m_Position.X, pBlock->m_Position.Y - BLOCK_SIZE);
                item.SetBlock(pBlock);
                
                int numItem = ((NONE == pBlock->m_Abilty) ? 0 : ((BREAKABLE == pBlock->m_Abilty) ? 4 : 1 ));
                for (int i = 0; i < numItem; i++) {
                    item.SetBrickPaertNum(i);
                    short xPos = pBlock->m_Position.X + ((i >> 1) ? BLOCK_SIZE_SMALL : 0);
                    short yPos = pBlock->m_Position.Y - BLOCK_SIZE - ((i & 1) ? BLOCK_SIZE_SMALL : 0);
                    
                    item.SetPosition(xPos, yPos);
                    m_lItem.push_back(item);
                    LogDebug (BIT_MARIO, "Mario::DrawBlocks(), Num Items %d \n", m_lItem.size());
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : polling is done
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::PolEvent() {
//    m_TimeLeft = m_pPlayer->GetState();
//    m_Score = m_pPlayer->GetPrevState();
    while (m_Window.pollEvent(m_Event)) {
        switch (m_Event.type) {
            case sf::Event::Closed:
                LogWarning (BIT_MARIO, "Mario::PolEvent(), Window is closed, closing the game !!!\n");
                m_Window.close();
                break;
                
            case sf::Event::KeyPressed:
                if (m_Event.key.code == sf::Keyboard::Right) {
                    m_pPlayer->Move(Entity::RIGHT, m_FrameShiftX);
                }
                else if (m_Event.key.code == sf::Keyboard::Left) {
                    m_pPlayer->Move(Entity::LEFT, m_FrameShiftX);
                }
                if ((m_Event.key.code == sf::Keyboard::LShift) && (Entity::GROUND ==  m_pPlayer->GetState())) {
                    m_pPlayer->SetState(Entity::JUMPING);
                    m_Sound.setBuffer(m_JumpSound);
                    m_Sound.play();
                }
                if (m_Event.key.code == sf::Keyboard::LControl) {
                    m_pPlayer->Fire(m_lBullet);
                }
                break;
                
            case sf::Event::KeyReleased:
                if ((m_Event.key.code == sf::Keyboard::Right) || (m_Event.key.code == sf::Keyboard::Left)) {
                    m_pPlayer->ResetPlayerMoveIdx();
                    m_pPlayer->SetImgIdx(PlayerImgIdx::STAND);
                    m_pPlayer->SetSpeed(Entity::DEFAULT_SPEED);
                    m_pPlayer->SetPrevState(Entity::RUNNING);
                }
                else if (m_Event.key.code == sf::Keyboard::LControl) {
                    m_pPlayer->SetShootingDone();
                    m_pPlayer->SetImgIdx(PlayerImgIdx::STAND);
                }
                break;
                                
//            case sf::Event::MouseMoved:
//                m_CoinCoint = ((m_Event.mouseMove.y * VIEW_HEIGHT) / m_Window.getSize().y);
//                m_Score = m_FrameShiftX + ((m_Event.mouseMove.x * VIEW_WIDTH) / m_Window.getSize().x);
//
//                m_TimeLeft = Obstacle::GetInstance()->GetObstacle(m_CoinCoint, m_Score);
//                break;
                
            default:
                break;
        } /// switch (m_Event.type)
    } /// while (m_Window.pollEvent(m_Event))
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : As Game starts Black screen showing Life of player
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::StartScreen() {
    if (!m_Texture.loadFromFile(gResourcePath + "graphics/StartScreen.png")) {
        LogError (BIT_MARIO, "Mario::PlayGame(), Can Not Load StartScreen Image \n");
        return;
    }
    m_Sprite.setTexture(m_Texture);
    short timeInSeconds = 0;
    while ((m_Window.isOpen()) && (timeInSeconds < 2)) {   /// Wait for 4 Seconds
        timeInSeconds = m_Clock.getElapsedTime().asSeconds();
        
        m_Window.clear();
        m_View.reset(sf::FloatRect(0.0f, 0.0f, VIEW_WIDTH, VIEW_HEIGHT));
        
        while (m_Window.pollEvent(m_Event)) {
            switch (m_Event.type) {
                case sf::Event::Closed:
                    LogWarning (BIT_MARIO, "Mario::PolEvent(), Window is closed, closing the game !!!\n");
                    m_Window.close();
                    break;

                default:
                    break;
            } /// switch (m_Event.type)
        }
        m_View.move(0, 0);
        m_Window.setView(m_View);
        m_Window.draw(m_Sprite);
        
        PrintContents(m_Score, m_CoinCoint, m_TimeLeft);
        DrawPlayerDummy(100, 130);
        
        PrintText("WORLD 1-1 \n\n     x  ", m_pPlayer->GetLife(), 400, 450);
        m_Window.display();
    }
    if (!m_Texture.loadFromFile(gResourcePath + "graphics/level1.png")) {
        LogError (BIT_MARIO, "Mario::PlayGame(), Can Not Load StartScreen Image \n");
        return;
    }
    m_Sprite.setTexture(m_Texture);
}

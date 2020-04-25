#include "Mario.h"
#include "Logger.h"
#include "Timer.h"
#include <algorithm>

const short gNumRowToDraw = 2;
const short gNumColView = VIEW_WIDTH >> BLOCK_SIZE_BIT;     /// to divide by Block SIze = 16
const std::array<short, gNumRowToDraw> gRowBlockArr {{gRow5ForBlock, gRow9ForBlock}};

std::array<EnemyPosType_s, 8> gEnemyPos {{{DUCK, 288}, {DOG, 480}, {DOG, 496}, {DOG, 832}, {DUCK, 1328}, {DUCK, 1360}, {DUCK, 1742}, {DOG, 2680}}};

void Mario::AddEnemy() {
    Enemy enemy;
    enemy.SetDirection(Entity::LEFT);
    
    for (auto &it : gEnemyPos) {
        enemy.SetPosition(it.xPos, 10);
        enemy.SetType(it.type);
        m_lEnemy.push_back(enemy);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mario::Mario()
: m_bPause(false), m_Score(0), m_CoinCoint(0), m_TimeLeft(MAX_TIME), m_FrameShiftX(0), m_pObstacle(nullptr), m_pPlayer(nullptr) {
    m_pObstacle = Obstacle::GetInstance();
    m_pPlayer = Player::GetInstance();
    
    AddEnemy();
    
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
        LogError (BIT_MARIO, "Mario::PlayGame(), Can not load Mariio Theme Music \n");
        return;
    }
    StartScreen();
    m_Music.play();
    
    m_Clock.restart();
    while (m_Window.isOpen()) {
        m_TimeLeft = MAX_TIME - m_Clock.getElapsedTime().asSeconds();
        
        if (m_pPlayer->GetIsDying()) {
            m_Music.stop();
        }
        
        ResetScreen();
        PolEvent();
        
        DrawView();
        DrawBlocks();

        if (Entity::INVISIBLE_STATE == m_pPlayer->GetState()) {
            m_bPause = true;
        }
        
        PausePlayer();

        DrawPlayer();

        DrawItems();
        DrawEnemy();
        
        CheckPlayerKill();
        
        PrintContents(m_Score, m_CoinCoint, m_TimeLeft);
        
        m_Window.display();
    } /// while (m_WinMario.isOpen())
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : on getting Nonus pause the player for a while
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::PausePlayer() {
    if (m_bPause) {
        sf::Clock clock;
        clock.restart();
        short pauseTime = 0;
        TileMap map;
        
        if (Entity::FIRABLE == m_pPlayer->GetAbility()) {
            while (pauseTime < 1000) {
                PausePlayerLoop(m_pPlayer->GetImgIdx(), Entity::FIRABLE + (pauseTime % 2) * 48, map);
                pauseTime = clock.getElapsedTime().asMilliseconds();
            }
        }
        else {
            while (pauseTime < 1000) {
                PausePlayerLoop((pauseTime % 2) * PlayerImgIdx::MID_INC, 0, map);
                pauseTime = clock.getElapsedTime().asMilliseconds();
            }
        }
        m_bPause = false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : printing player on pause loop
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::PausePlayerLoop(const short imgX, const short imgY, TileMap &map) {
    m_Window.clear();
    m_View.reset(sf::FloatRect(0.0f, 0.0f, VIEW_WIDTH, VIEW_HEIGHT));
    m_View.move(m_FrameShiftX, 0);
    m_Window.setView(m_View);
    m_Window.draw(m_Sprite);

    DrawBlocks();

    TileMap::PrintControl_s printControl;
    printControl.imgXIdx = imgX;
    printControl.imgYIdx = imgY;
    printControl.position = m_pPlayer->GetPosition();
    printControl.xTileOffset = BLOCK_SIZE_BIT;
    printControl.tileSize = sf::Vector2<float>(BLOCK_SIZE, (BLOCK_SIZE << 1));
    printControl.imgType = TileMap::PLAYER;
    printControl.bIsInverted = (Entity::RIGHT == m_pPlayer->GetDirection()) ? false : true;

    map.Load(printControl);
    m_Window.draw(map);

    PrintContents(m_Score, m_CoinCoint, m_TimeLeft);
    m_Window.display();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Check is player killed if yes the reset the game
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario:: CheckPlayerKill() {
    if (m_pPlayer->GetIsLifeReduced()) {
        StartScreen();
        m_Music.play();
        m_FrameShiftX = 0;
        m_pPlayer->Reset();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw Enemy
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Mario::DrawEnemy() {
    auto it = m_lEnemy.begin();
    for (; it != m_lEnemy.end(); it++) {
        if ((it->GetPosition().X - m_FrameShiftX) < 320) {
            it->LoadEnemyImage(m_Window);
        }
        if (Entity::DYING == it->GetState()) {
            m_lEnemy.erase(it);
            LogInfo(BIT_MARIO, "Mario::DrawEnemy(), Num Enemy %d \n", m_lEnemy.size());
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
                m_bPause = true;
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
                    m_pPlayer->Fire();
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
    m_StartScreenClock.restart();
    m_Music.stop();
    short timeInSeconds = 0;
    while ((m_Window.isOpen()) && (timeInSeconds < 2)) {   /// Wait for 4 Seconds
        timeInSeconds = m_StartScreenClock.getElapsedTime().asSeconds();
        
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

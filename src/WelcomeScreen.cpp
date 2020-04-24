#include "WelcomeScreen.h"
#include "Logger.h"
#include <array>

const std::array<short, 2> gSelectYpos {{144, 154}};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WelcomeScreen::WelcomeScreen()
: m_Window({WIN_WIDTH, WIN_HEIGHT}, "Super MArio"),
m_SelectPos(gSelectYpos[0]) {
    LogInfo(BIT_WELCOME_SCREEN, "WelcomeScreen::WelcomeScreen(), Destructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WelcomeScreen::~WelcomeScreen() {
    LogInfo(BIT_WELCOME_SCREEN, "WelcomeScreen::~WelcomeScreen(), Destructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Loads all the required contents
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool WelcomeScreen::LoadRenderGraphics() {
    if (!m_Texture.loadFromFile(gResourcePath + "graphics/level1.png")) {
        LogError (BIT_WELCOME_SCREEN, "WelcomeScreen::LoadRenderGraphics(), Can Not Load the Level 1 Image \n");
        return false;
    }
    m_Sprite.setTexture(m_Texture);
    
    if (!m_Font.loadFromFile(gResourcePath + "fonts/Mario.ttf")) {
        LogError (BIT_WELCOME_SCREEN, "WelcomeScreen::LoadRenderGraphics(), Can Not Load Fonts  exiting Game \n");
        return false;
    }
    m_Text.setFont(m_Font);
    m_Text.setCharacterSize(32); /// text Size is 30;
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Print the texts
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WelcomeScreen::PrintContents(const short score, const short coinCount, const short time, const short yPos) {
    ///Draw the Small Coin icon in in score line
    static short i = 0;
    ++i %= 3;
    TileMap::PrintControl_s printControl;
    printControl.imgXIdx = i;
    printControl.imgYIdx = 160;                         /// as per BLOCKSIZE_SMALL Small coin is at 20 (20 * 8) offset in image
    printControl.position = Position_s(88, 25);
    printControl.xTileOffset = BLOCK_SIZE_SMALL_BIT;
    printControl.tileSize = sf::Vector2<float>(BLOCK_SIZE_SMALL, BLOCK_SIZE_SMALL);
    printControl.imgType = TileMap::ITEM;
    m_Map.Load(printControl);
    
    m_View.reset(sf::FloatRect{0, 0, VIEW_WIDTH, VIEW_HEIGHT});
    m_Window.setView(m_View);
    m_Window.draw(m_Map);

    /// Prints the score and time
    short xPos = 88;                              /// Print First string at offset of 80 from left
    PrintText("MARIO\n ", score, xPos, yPos);
    PrintText("\n  x ", coinCount, (xPos += 256), yPos);
    PrintText("WORLD\n 1-", 1, (xPos += 256), yPos);       /// Only Level 1
    PrintText("TIME\n ", time, (xPos += 256), yPos);       /// Time Limit for Level 1 is 400
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Store the text and valu ein string and print it
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WelcomeScreen::PrintText(const char *printStuff, const short value, const short xPos, const short yPos) {
    char *printString = new char[20];
    sprintf(printString, "%s%d", printStuff, value);
    
    sf::View textView;
    m_Window.setView(textView);

    m_Text.setPosition(xPos, yPos);       /// From top print at 40 Offset
    m_Text.setString(printString);
    
    m_Window.draw(m_Text);
    m_Window.setView(m_View);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Store the text and valu ein string and print it
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WelcomeScreen::PrintText(const char *printStuff, const short xPos, const short yPos) {
    sf::View textView;
    m_Window.setView(textView);
    
    m_Text.setPosition(xPos, yPos);       /// From top print at 40 Offset
    m_Text.setString(printStuff);
    
    m_Window.draw(m_Text);
    m_Window.setView(m_View);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw Welcom screen to selec the game type
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WelcomeScreen::DrawWelcomeScreen() {
    LogInfo(BIT_WELCOME_SCREEN, "WelcomeScreen::DrawWelcomeScreen(), Drawing Welcome Screen !!! \n");
    if (!LoadRenderGraphics()) {
        return;
    }
    
    if (!m_WelcomeTexture.loadFromFile(gResourcePath + "graphics/SuperMario.png")) {
        LogError(BIT_WELCOME_SCREEN, "WelcomeScreen::DrawWelcomeScreen(), Drawing Welcome Screen !!! \n");
        return;
    }
    m_WelcomeSprite.setTexture(m_WelcomeTexture);
    
    auto ResetScreen = [&]() {
        m_Window.clear();
    };
    
    auto DrawView = [&]() {
        m_View.move(0, 0);
        m_View.reset(sf::FloatRect(0.0f, 0.0f, VIEW_WIDTH, VIEW_HEIGHT));
        m_Window.setView(m_View);
        m_Window.draw(m_Sprite);
    };

    while (m_Window.isOpen()) {
        ResetScreen();
        PolEvent();
            
        DrawView();
        PrintContents(0, 0, MAX_TIME);
        PrintWelcomeContent();
        DrawPlayerDummy(40, 200);
        
        m_Window.display();
    } /// while (m_WinMario.isOpen())
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw Welcome Screen and Print Select Player
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WelcomeScreen::PrintWelcomeContent() {
    m_WelcomeSprite.setPosition(40, 40);
    m_Window.draw(m_WelcomeSprite);

    TileMap::PrintControl_s printControl;
    printControl.imgXIdx = 3;
    printControl.imgYIdx = 20 << BLOCK_SIZE_SMALL_BIT;
    printControl.position = Position_s(80, m_SelectPos);
    printControl.xTileOffset = BLOCK_SIZE_SMALL_BIT;
    printControl.tileSize = sf::Vector2<float>(BLOCK_SIZE_SMALL, BLOCK_SIZE_SMALL);
    printControl.imgType = TileMap::ITEM;
    
    m_Map.Load(printControl);
    m_Window.draw(m_Map);

    PrintText(" I KAN ", 620, 550);
    PrintText("1 PLAYER GAME ", 350, 600);
    PrintText("1 PLAYER GAME ", 350, 650);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw dummy player in welcomwe screen
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WelcomeScreen::DrawPlayerDummy(const short x, const short y) {
    TileMap::PrintControl_s printControl;
    printControl.imgXIdx = 0;
    printControl.imgYIdx = 0 * 48 + 32;                          /// 48 :  Sum of Height of player Small and Big, 32 : Height of player BIG
    printControl.position = Position_s(x, y);
    printControl.xTileOffset = BLOCK_SIZE_BIT;
    printControl.tileSize = sf::Vector2<float>(BLOCK_SIZE, BLOCK_SIZE);
    printControl.imgType = TileMap::PLAYER;
    printControl.bIsInverted = true;
    
    m_Map.Load(printControl);
    m_Window.draw(m_Map);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : polling is done
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WelcomeScreen::PolEvent() {
    while (m_Window.pollEvent(m_Event)) {
        switch (m_Event.type) {
            case sf::Event::KeyPressed:
                if (m_Event.key.code == sf::Keyboard::S) {
                    LogWarning (BIT_WELCOME_SCREEN, "WelcomeScreen::PolEvent(), Window is closed, closing the game !!!\n");
                    m_Window.close();
                }
                else if (m_Event.key.code == sf::Keyboard::A) {
                    static short i = 0;
                    i ^= 1;
                    m_SelectPos = gSelectYpos[i];
                }
                break;
            default:
                break;
        } /// switch (m_Event.type)
    } /// while (m_Window.pollEvent(m_Event))
}

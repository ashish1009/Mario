#pragma once
#include "SfmlRenders.h"
#include "TileMap.h"

class WelcomeScreen : protected SFML {
public:
    /// View * 4 (4 just randon number it can be anything)
    static constexpr short WIN_WIDTH = VIEW_WIDTH << 2;
    /// View * 4 (4 just randon number it can be anything)
    static constexpr short WIN_HEIGHT = VIEW_HEIGHT << 2;
    
    /// Max Time limit for Level 1
    static constexpr short MAX_TIME = 400;

public:
    WelcomeScreen();
    ~WelcomeScreen();
    
    void DrawWelcomeScreen();

private:
    short m_SelectPos;
    
    sf::Texture m_WelcomeTexture;
    sf::Sprite m_WelcomeSprite;
    
    TileMap m_Map;
    
protected:
    /// Render Window for the window that appears in game
    sf::RenderWindow m_Window;
    
    /// Render View to provide the view in rectangle for background level 1
    sf::View m_View;
    
protected:
    virtual void PolEvent();
    
    bool LoadRenderGraphics();
    void PrintContents(const short score, const short coinCount, const short time, const short yPos = 40);
    void PrintText(const char *printStuff, const short value, const short xPos, const short yPos);
    void PrintText(const char *printStuff, const short xPos, const short yPos);
    void DrawPlayerDummy(const short x, const short y);
    
private:
    void PrintWelcomeContent();
};

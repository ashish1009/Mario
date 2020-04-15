#pragma once
#include <iostream>
#include "Obstacle.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

/// Path for Images used in Game
const std::string ResourcePath = "/Users/ashish./Documents/XCode/Mario/Mario/Resources/";
const std::string BLOCK_IMG_PATH = "Graphics/Item.png";

/// Window Size Factro From World View
const short WINDOW_WORLD_FACTOR = 4;

/// Mario World View size WORLD_VIEW_WIDTH X WORLD_VIEW_HEIGHT
const short WORLD_VIEW_HEIGHT = 224;
const short WORLD_VIEW_WIDTH = 256;

/// Window Pixels WIN_WIDTH X WIN_HEIGHT
const short WIN_HEIGHT = WORLD_VIEW_HEIGHT * 4;
const short WIN_WIDTH = WORLD_VIEW_WIDTH * 4;

const short BLOCK_SIZE_BIT = 4;

/// Num of Col in terms of Blocks
const int gNumColView = WORLD_VIEW_WIDTH >> BLOCK_SIZE_BIT;     /// to divide by Block SIze = 16

class SoundBuffer {
protected:
    sf::Sound m_Sound;
    
    sf::SoundBuffer m_JumpSound;
    sf::SoundBuffer m_BrickSmashSound;
    sf::SoundBuffer m_BumpSound;
    sf::SoundBuffer m_CoinSound;
    sf::SoundBuffer m_CountDownSound;
    sf::SoundBuffer m_FireSound;
    sf::SoundBuffer m_KickSound;
    sf::SoundBuffer m_OneUpSound;
    sf::SoundBuffer m_PowerUpAppearSound;
    sf::SoundBuffer m_PowerUpSound;
    sf::SoundBuffer m_StompSound;
    sf::SoundBuffer m_GameOverSound;    

public:
    SoundBuffer();
    ~SoundBuffer();
};

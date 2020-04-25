#pragma once
#include <Common.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

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
    
    sf::SoundBuffer m_DeathSound;    

public:
    SoundBuffer();
    ~SoundBuffer();
};

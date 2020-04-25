#include "SoundBuffer.h"
#include "Logger.h"

SoundBuffer::SoundBuffer() {
    LogInfo(BIT_BONUS, "SoundBuffer::SoundBuffer() : Constructor called !!!\n");
    m_JumpSound.loadFromFile(gResourcePath + "sound/big_jump.ogg");
    m_BrickSmashSound.loadFromFile(gResourcePath + "sound/brick_smash.ogg");
    m_BumpSound.loadFromFile(gResourcePath + "sound/bump.ogg");
    m_CoinSound.loadFromFile(gResourcePath + "sound/coin.ogg");
    m_CountDownSound.loadFromFile(gResourcePath + "sound/count_down.ogg");
    m_FireSound.loadFromFile(gResourcePath + "sound/fireball.ogg");
    m_KickSound.loadFromFile(gResourcePath + "sound/kick.ogg");
    m_OneUpSound.loadFromFile(gResourcePath + "sound/one_up.ogg");
    m_PowerUpAppearSound.loadFromFile(gResourcePath + "sound/powerup_appears.ogg");
    m_PowerUpSound.loadFromFile(gResourcePath + "sound/powerup.ogg");
    m_DeathSound.loadFromFile(gResourcePath + "music/death.wav");
}

SoundBuffer::~SoundBuffer() {
    LogInfo(BIT_BONUS, "SoundBuffer::~SoundBuffer() : Destructor called !!!\n");
}

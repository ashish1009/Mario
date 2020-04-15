#include "Common.h"
#include "Logger.h"

SoundBuffer::SoundBuffer() {
//    LogInfo(BIT_BONUS, "SoundBuffer::SoundBuffer() : Constructor called \n");
    m_JumpSound.loadFromFile(ResourcePath + "sound/big_jump.ogg");
    m_BrickSmashSound.loadFromFile(ResourcePath + "sound/brick_smash.ogg");
    m_BumpSound.loadFromFile(ResourcePath + "sound/bump.ogg");
    m_CoinSound.loadFromFile(ResourcePath + "sound/coin.ogg");
    m_CountDownSound.loadFromFile(ResourcePath + "sound/count_down.ogg");
    m_FireSound.loadFromFile(ResourcePath + "sound/fireball.ogg");
    m_KickSound.loadFromFile(ResourcePath + "sound/kick.ogg");
    m_OneUpSound.loadFromFile(ResourcePath + "sound/one_up.ogg");
    m_PowerUpAppearSound.loadFromFile(ResourcePath + "sound/powerup_appears.ogg");
    m_PowerUpSound.loadFromFile(ResourcePath + "sound/powerup.ogg");
}

SoundBuffer::~SoundBuffer() {
//    LogInfo(BIT_BONUS, "SoundBuffer::~SoundBuffer() : Destructor called \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Create new Instance for Bonus. If alredy created then return the older Pointer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Entity.h"
#include "Bullet.h"
#include "SoundBuffer.h"
#include <list>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Player Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Player : public Entity, SoundBuffer {
private:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Player Image Size in Pixel
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static constexpr short PLAYER_WIDTH = 16;
    static constexpr short PLAYER_HEIGHT_SMALL = 16;
    static constexpr short PLAYER_HEIGHT_BIG = 32;
    static constexpr short TOTAL_HEIGHT = PLAYER_HEIGHT_SMALL + PLAYER_HEIGHT_BIG;
    
    static constexpr float MAX_JUMP_HEIGHT = 70;
    static constexpr short DEFAULT_PLAYER_LIFE = 3;

private:
    static Player *m_Instance;
    
    bool m_bShotFire;
    bool m_bLifeReduced;
    
    short m_Life;
    short m_ImgIdx;
    short m_PlayerMoveIdx;
    
    float m_JumpFactor;
    
private:
    Player();
    void CheckState();
    
public:
    static Player *GetInstance();
    static Player *ReleaseInstance();
    
    void LoadImage(sf::RenderWindow &window);
    void Move(Entity::Direction_e directiom, float &frameX);
    void Jump();
    void Fire(std::list<Bullet> &fireList);
    void Reset();
    
    inline bool GetIsLifeReduced () const {
        return m_bLifeReduced;
    }
    inline void SetIsLifeReduced() {
        m_bLifeReduced = false;
    }
    inline short GetLife() const {
        return m_Life;
    }
    inline void SetImgIdx(const short imgIdx) {
        m_ImgIdx = imgIdx;
    }
    inline void ResetPlayerMoveIdx() {
        m_PlayerMoveIdx = 0;
    }
    inline void SetShootingDone () {
        m_bShotFire = true;
    }    
    inline void IncSpeed() {
        if (4 > m_Speed) {
            m_Speed++;
        }
    }
    inline short GetImgIdx() {
        return m_ImgIdx;
    }
    ~Player();
};

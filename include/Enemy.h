#pragma once
#include "Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Enemy Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Enemy : public Entity, private SoundBuffer {
private:
    static inline const std::string PLAYER_IMG_PATH = "Graphics/Mario.png";

public:
    Enemy();
    ~Enemy();
    
    int LoadPlayerImage(sf::RenderWindow &winMario);
    /// Pure virtual funxtion
    bool IsDownCollision (const int frameX) override;
    bool IsJumpCollision (const int frameX) override;
    bool IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) override;
    bool IsPlayerCollision () override;
};

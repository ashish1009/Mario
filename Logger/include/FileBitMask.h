#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Bit Mask for Logging
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum FileBitMask_e
{
    /// first Nibble (LSB)
    BIT_MAIN = 0,
    BIT_MARIO = 1,
    BIT_ENTITY = 2,
    BIT_PLAYER = 3,

    /// second Nibble
    BIT_TILEMAP = 4,
    BIT_BLOCK = 5,
    BIT_OBSTACLE = 6,
    BIT_LIST = 7,

    /// third Nibble 
    BIT_BONUS = 8,
    BIT_ITEM = 9,
    BIT_BULLET = 10,
    BIT_ENEMY = 11,
    
    /// forth Nibble
    BIT_WELCOME_SCREEN = 12,
    BIT_SFML = 13,
};

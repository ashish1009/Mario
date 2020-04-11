#pragma once

#include <iostream>
#include <vector>
#include "Entity.h"

const short NUM_COL = 212;
const short NUM_ROW = 15;
const short BLOCK_SIZE = 16;

const short gPixelToBeLandedL = 3;
const short gPixelToBeLandedR = BLOCK_SIZE - gPixelToBeLandedL;

const short gPixelToBeUpColloidedL = 5;
const short gPixelToBeUpColloidedR = BLOCK_SIZE - gPixelToBeUpColloidedL;

const short gPixelToColloidD = 5;               /// First pixel from Down to be collided : can be changed

const short gRow9ForBlock = 9;
const short gRow5ForBlock = 5;

const short gNumRowToDraw = 2;
const short gRowBlockArr[gNumRowToDraw] = {gRow5ForBlock, gRow9ForBlock};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obstacle Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Obstacle {
public:
    enum Behaviour_e : short {
        NO_BEHAV = -1,
        BRICK = 3,
        BONUS = 4,
        END_FLAG = 7,
        BASE = 8,
        SCORE = 9,
        STEP = 10,
        PIPE = 11,
        PIPE_BASE = 12,
        BASE_HALF = 13,
        PRINCES = 14,
        BLANK = 17,
    };
    
    enum Abilities_e : short{
        NO_ABILITY = -1,
        MUSHROOM = 0,
        FIRE_LEAFE = 1,
        STAR = 2,
        COIN = 5,
        COIN_TILT = 6,
        FIRE_BULLET = 18,
    };
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Strcture for each Pixel
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct ObstaclePixel_s {
        bool bIsObstacle;
        Behaviour_e behaviour;
        Abilities_e abilities;
        
        ObstaclePixel_s()
        :bIsObstacle(false), behaviour(NO_BEHAV), abilities(NO_ABILITY) {
        }
        
        void SetObstacle (bool bIsObstacle, Behaviour_e behaviour, Abilities_e abilities, int size = BLOCK_SIZE) {
            this->bIsObstacle = bIsObstacle;
            this->behaviour = behaviour;
            this->abilities = abilities;
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Strcture for each Block
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct ObstacleBlock_s {
        int xPos;       /// x Position of Block relative to frame in Pixel
        int yPos;       /// y Position of Block relative to frame in Pixel
        short size;
        short upPopped;
        Behaviour_e behaviour;
        Abilities_e abilities;
        bool bIsObstacle;
        bool bIsPopped;
        bool bIsEmpty;
        
        ObstacleBlock_s()
        :bIsObstacle(false), bIsPopped(false), bIsEmpty(false), behaviour(NO_BEHAV), abilities(NO_ABILITY), size(BLOCK_SIZE), upPopped(0) {
        }
    };
    
private:
    static Obstacle *m_pInstance;
    
    std::vector<std::vector<ObstaclePixel_s>> m_Obstacle;
    
    ///Row' refer to position Y and column refer to position X
    ObstacleBlock_s m_BlockType[NUM_ROW][NUM_COL];
    
private:
    Obstacle();
    ~Obstacle();
    
    void InitObstacles();
    
public:
    static Obstacle *GetInstance();
    static Obstacle *ReleaseInstance();
    
    void ResetBlockPixels(const unsigned int row, const unsigned int col);
    
    void SetObstaclePixels(const unsigned int row, const unsigned int col);
    void SetBlock(const unsigned int row, const unsigned int col);
    void SetObstacle(const int unsigned X, const unsigned int Y, Behaviour_e behaviour, Abilities_e ability);
    
    Behaviour_e GetBlockTypeAt(const unsigned int i, const unsigned int j) const;
    Abilities_e GetBlockAbilityAt(const unsigned int i, const unsigned int j) const;
    int GetBlockSizeAt(const unsigned int i, const unsigned int j) const;
    bool GetIsObstacleAt(const unsigned int i, const unsigned int j) const;
     
    void SetPoppedBlock(const unsigned int row, const unsigned int col);
    bool IsPoppedBlock(const unsigned int i, const unsigned int j) const;
    
    ObstacleBlock_s *GetBlockReference(const unsigned int i, const unsigned int j);
};


#pragma once

#include <iostream>
#include <vector>
#include "Common.h"
#include "Entity.h"

const unsigned short BASIC_PIXEL_SIZE = 1;

const unsigned short FRAME_PIXEL_ARRAY_X = LEVEL_1_WIDTH / BASIC_PIXEL_SIZE; // 7
const unsigned short FRAME_PIXEL_ARRAY_Y = WIN_HEIGHT / BASIC_PIXEL_SIZE; // 12

class Obstacle
{
public:
    enum Behaviour_e
    {
        NONE = 0,
        COIN = 1,
    };
    
    struct Obstacle_s
    {
        bool bIsObstacle;
        Behaviour_e Behaviour;
        
        Obstacle_s()
        : bIsObstacle(false), Behaviour(NONE){}
        
        void SetObstacle(bool flag, Behaviour_e behav);
    };
    
private:
    static Obstacle *m_pInstance;
    
public:
    std::vector<std::vector<Obstacle_s>> m_bIsObstacle;

private:
    Obstacle();
    ~Obstacle();

public:
    static Obstacle *GetInstance();
    static void ReleaseInstance();

    inline const bool IsEmptyFramePixel(const unsigned short X, const unsigned short Y) const
    {
        return m_bIsObstacle[X][Y].bIsObstacle;
    }
    
    inline void PushLastColumnPixels(const std::vector<Obstacle_s> &ColPixels)
    {
        m_bIsObstacle.push_back(ColPixels);
    }
    
    inline void PopFirstColumnPixels()
    {
        m_bIsObstacle.erase(m_bIsObstacle.begin());
    }
    
    void SetObstacle(const unsigned short X, const unsigned short Y, Behaviour_e Behaviour);
};

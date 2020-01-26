#include "Obstacle.h"
#include "Logger.h"

#define HOLLOW_POS_1 1104
#define HOLLOW_POS_2 1376
#define HOLLOW_POS_3 2448
#define HOLLOW_WIDTH 32

Obstacle *Obstacle::m_pInstance = nullptr;

bool IsHollow(int Index)
{
    if (((Index >= HOLLOW_POS_1) && (Index <= HOLLOW_POS_1 + HOLLOW_WIDTH)) ||
    ((Index >= HOLLOW_POS_2) && (Index <= HOLLOW_POS_2 + HOLLOW_WIDTH)) ||
    ((Index >= HOLLOW_POS_3) && (Index <= HOLLOW_POS_3 + HOLLOW_WIDTH)))
    {
        LogDebug(BIT_OBSTACLE, "%d \n", Index);
    }
    
    return (((Index >= HOLLOW_POS_1) && (Index <= HOLLOW_POS_1 + HOLLOW_WIDTH)) ||
            ((Index >= HOLLOW_POS_2) && (Index <= HOLLOW_POS_2 + HOLLOW_WIDTH)) ||
            ((Index >= HOLLOW_POS_3) && (Index <= HOLLOW_POS_3 + HOLLOW_WIDTH)));
};

Obstacle::Obstacle()
{
    for (int i = 0; i < FRAME_PIXEL_ARRAY_X; i++)
    {
        std::vector<bool> colPixels;
        for (int j = 0; j < FRAME_PIXEL_ARRAY_Y; j++)
        {
            if((GROUND_LEVEL <= j) && (!IsHollow(i)))
                colPixels.push_back(true);
            else
                colPixels.push_back(false);
        }
        m_bIsObstacle.push_back(colPixels);
    }
    LogDebug(BIT_OBSTACLE, "Obstacle::Obstacle(), Constructor !! \n");
}

Obstacle::~Obstacle()
{
//    LogDebug(BIT_OBSTACLE, "Obstacle::~Obstacle(), Destructor !! \n");
}

Obstacle *Obstacle::GetInstance()
{
    if (nullptr == m_pInstance)
    {
        LogInfo(BIT_OBSTACLE, "Obstacle::GetInstance(), Creating Obstacle Instance() \n");
        m_pInstance = new Obstacle;
    }
    return m_pInstance;
}

void Obstacle::ReleaseInstance()
{
    if (nullptr != m_pInstance)
    {
        LogInfo(BIT_OBSTACLE, "Obstacle::ReleaseInstance(), Deleting Obstacle Instance() \n");
        delete m_pInstance;
    }
}


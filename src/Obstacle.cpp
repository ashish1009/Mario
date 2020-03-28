#include "Obstacle.h"
#include "Logger.h"

Obstacle *Obstacle::m_pInstance = nullptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Obstacle Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle::Obstacle() {
    for (int i = 0; i < NUM_ROW * BLOCK_SIZE; i++) {
        std::vector<Obstacle_s> colPixels;
        Obstacle_s obstacle;
        
        for (int j = 0; j < NUM_COL * BLOCK_SIZE; j++) {
            obstacle.SetObstacle(false, NO_BEHAV, NO_ABILITY);
            colPixels.push_back(obstacle);
        }
        m_Obstacle.push_back(colPixels);
    }
    
    InitObstacles();
    LogInfo(BIT_OBSTACLE, "Obstacle::Obstacle(), Constructor !! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Obstacle Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle::~Obstacle() {
    LogInfo(BIT_OBSTACLE, "Obstacle::~Obstacle(), Destructor !! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Create Instace of Obstacle if not created otherwise return the created one
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle *Obstacle::GetInstance() {
    if (nullptr == m_pInstance) {
        LogInfo(BIT_OBSTACLE, "Obstacle::GetInstance(), Creating Obstacle Instance() \n");
        m_pInstance = new Obstacle;
    }
    return m_pInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Delete the Instance if created
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::ReleaseInstance() {
    if (nullptr != m_pInstance) {
        LogInfo(BIT_OBSTACLE, "Obstacle::ReleaseInstance(), Deleting Obstacle Instance() \n");
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Get The Block type at row i and col j
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle::Behaviour_e Obstacle::GetBlockTypeAt(const int i, const int j) const {
    if (((i < NUM_ROW) && (j < NUM_COL)) && (i >= 0 && j >= 0)) {
        return m_BlockType[i][j].behaviour;
    }
    else {
        LogError (BIT_OBSTACLE, "Obstacle::GetBlockTypeAt(%d, %d) :  invalid row %d or col %d \n", NUM_ROW, NUM_COL, i, j);
        return Obstacle::NO_BEHAV;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Get The Block Ability at row i and col j
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle::Abilities_e Obstacle::GetBlockAbilityAt(const int i, const int j) const {
    if (((i < NUM_ROW) && (j < NUM_COL)) && (i >= 0 && j >= 0)) {
        return m_BlockType[i][j].abilities;
    }
    else {
        LogError (BIT_OBSTACLE, "Obstacle::GetBlockAbilityAt(%d, %d) :  invalid row %d or col %d \n", NUM_ROW, NUM_COL, i, j);
        return Obstacle::NO_ABILITY;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Get The Block type at row i and col j
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Obstacle::GetBlockSizeAt(const int i, const int j) const {
    if (((i < NUM_ROW) && (j < NUM_COL)) && (i >= 0 && j >= 0)) {
        return m_BlockType[i][j].size;
    }
    else {
        LogError (BIT_OBSTACLE, "Obstacle::GetBlockSizeAt(%d, %d) :  invalid row %d or col %d \n", NUM_ROW, NUM_COL, i, j);
        return BLOCK_SIZE;
    }
}

/// return the Pixl stateus is Obstacle or not, if indexes are out of range then return false
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : return the Pixl stateus is Obstacle or not, if indexes are out of range then return false
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Obstacle::GetIsObstacleAt(const int i, const int j) const {
    if (((i < NUM_ROW * BLOCK_SIZE) && (j < NUM_COL * BLOCK_SIZE)) && (i >= 0 && j >= 0)) {
        return m_Obstacle[i][j].bIsObstacle;
    }
    else {
        LogError (BIT_OBSTACLE, "Obstacle::GetIsObstacleAt(%d, %d) :  invalid row %d or col %d \n", NUM_ROW * BLOCK_SIZE, NUM_COL * BLOCK_SIZE, i, j);
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Set the Pixels of the block at X and Y
///         Where X is Pos Y and Y is Pos X
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::SetObstacle(const int row, const int col, Behaviour_e behaviour, Abilities_e ability) {
    if (((row < NUM_ROW * BLOCK_SIZE) && (col < NUM_COL * BLOCK_SIZE)) && (row >= 0 && col >= 0)) {
        m_Obstacle[row][col].bIsObstacle = true;
        m_Obstacle[row][col].behaviour = behaviour;
        m_Obstacle[row][col].abilities = ability;
    }
    else {
        if ((col >= NUM_COL * BLOCK_SIZE) && (col < -8)) { /// For debug only
            LogError (BIT_OBSTACLE, "Obstacle::SetObstacle(%d, %d) :  invalid row %d or col %d \n", NUM_ROW * BLOCK_SIZE, NUM_COL * BLOCK_SIZE, row, col);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Add the last Column
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::PushLastColumnPixels() {
    std::vector<Obstacle_s> colPixels;
    Obstacle_s obstacle;
    
    for (int i = 0; i < NUM_ROW * BLOCK_SIZE; i++) {
        obstacle.SetObstacle(false, NO_BEHAV, NO_ABILITY);
        m_Obstacle[i].push_back(obstacle);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief       : Erase the first column
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::PopFirstColumnPixels() {
    for (int i = 0; i < NUM_ROW * BLOCK_SIZE; i++) {
        m_Obstacle[i].erase(m_Obstacle[i].begin());
    }
}

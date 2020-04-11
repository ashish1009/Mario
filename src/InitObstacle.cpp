#include "Obstacle.h"
#include "Logger.h"

const short MAX_HOLLOW_BLOCK = 7;
const short MAX_PIPE = 6;
const short MAX_STEP = 27;

const short MAX_BLOCKS = 21;

/// Constanct for defininging specific Block
const short gHollowColIdx[MAX_HOLLOW_BLOCK] = {69, 70, 86, 87, 88, 153, 154};
const short gPipeColIdx[MAX_PIPE] = {28, 38, 46, 57, 163, 179};
const short gPipeSize[MAX_PIPE] = {2, 3, 4, 4, 2, 2};
const short gStepColIdx[MAX_STEP] = {
    134, 135, 136, 137,
    140, 141, 142, 143,
    148, 149, 150, 151, 152,
    155, 156, 157, 158,
    181, 182, 183, 184, 185, 186, 187, 188, 189, 198};

const short gStepNum[MAX_STEP] = {1, 2, 3, 4, 4, 3, 2, 1, 1, 2, 3, 4, 4, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1};

const short gRow5ColIdx[MAX_BLOCKS] = {
    22,
    80, 81, 82, 83, 84, 85, 86, 87,
    91, 92, 93, 94,
    109,
    121, 122, 123,
    128, 129, 130, 131};

const Obstacle::Behaviour_e gRow5Behaviour[MAX_BLOCKS] = {
    Obstacle::BONUS,
    Obstacle::BRICK, Obstacle::BRICK, Obstacle::BRICK, Obstacle::BRICK, Obstacle::BRICK, Obstacle::BRICK, Obstacle::BRICK, Obstacle::BRICK,
    Obstacle::BRICK, Obstacle::BRICK, Obstacle::BRICK, Obstacle::BONUS,
    Obstacle::BONUS,
    Obstacle::BRICK, Obstacle::BRICK, Obstacle::BRICK,
    Obstacle::BRICK, Obstacle::BONUS, Obstacle::BONUS, Obstacle::BRICK
};

const Obstacle::Abilities_e gRow5Ability[MAX_BLOCKS] = {
    Obstacle::COIN,
    Obstacle::NO_ABILITY, Obstacle::NO_ABILITY, Obstacle::NO_ABILITY, Obstacle::NO_ABILITY, Obstacle::NO_ABILITY, Obstacle::NO_ABILITY, Obstacle::NO_ABILITY,
    Obstacle::NO_ABILITY, Obstacle::MUSHROOM, Obstacle::NO_ABILITY, Obstacle::STAR,
    Obstacle::MUSHROOM,
    Obstacle::NO_ABILITY, Obstacle::NO_ABILITY, Obstacle::NO_ABILITY,
    Obstacle::NO_ABILITY, Obstacle::COIN, Obstacle::COIN, Obstacle::NO_ABILITY
};

const short gRow9ColIdx[MAX_BLOCKS] = {
    16,
    20, 21, 22, 23,
    77, 78, 79,
    94,
    100, 101,
    106, 109, 112,
    118,
    129, 130,
    168, 169, 170, 171
};

const Obstacle::Behaviour_e gRow9Behaviour[MAX_BLOCKS] = {
    Obstacle::BONUS,
    Obstacle::BRICK, Obstacle::BONUS, Obstacle::BRICK, Obstacle::BONUS,
    Obstacle::BRICK, Obstacle::BONUS, Obstacle::BRICK,
    Obstacle::BRICK,
    Obstacle::BRICK, Obstacle::BRICK,
    Obstacle::BONUS, Obstacle::BONUS, Obstacle::BONUS,
    Obstacle::BRICK,
    Obstacle::BRICK, Obstacle::BRICK,
    Obstacle::BRICK, Obstacle::BRICK, Obstacle::BONUS, Obstacle::BRICK,
};

const Obstacle::Abilities_e gRow9Ability[MAX_BLOCKS] = {
    Obstacle::COIN,
    Obstacle::NO_ABILITY, Obstacle::COIN, Obstacle::NO_ABILITY, Obstacle::MUSHROOM,
    Obstacle::NO_ABILITY, Obstacle::MUSHROOM, Obstacle::NO_ABILITY,
    Obstacle::NO_ABILITY,
    Obstacle::NO_ABILITY, Obstacle::NO_ABILITY,
    Obstacle::COIN, Obstacle::COIN, Obstacle::COIN,
    Obstacle::NO_ABILITY,
    Obstacle::NO_ABILITY, Obstacle::NO_ABILITY,
    Obstacle::NO_ABILITY, Obstacle::NO_ABILITY, Obstacle::COIN, Obstacle::NO_ABILITY
};

const short gPipeRowIdx = 12;
const short gBaseGroundRowIdx = gPipeRowIdx + 1;

void Obstacle::SetBlock(const unsigned int row, const unsigned int col) {
    m_BlockType[row][col].size = BLOCK_SIZE;
    m_BlockType[row][col].behaviour = NO_BEHAV;
    m_BlockType[row][col].abilities = NO_ABILITY;
    m_BlockType[row][col].yPos = (row << BLOCK_SIZE_BIT) + (BLOCK_SIZE >> 1); /// row * 16 + 8        : Shift 8  for last half ground block

    if ((gBaseGroundRowIdx == row || (gBaseGroundRowIdx + 1) == row)) {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                 Base Ground and Hollow
        for (int i = 0; i < MAX_HOLLOW_BLOCK; i++) {
            if (gHollowColIdx[i] == col) {
                m_BlockType[row][col].behaviour = BLANK;
                return;
            }
        }
        m_BlockType[row][col].behaviour = BASE;
        SetObstaclePixels(row, col);
        return;
    } ///if ((gBaseGroundRowIdx == row || (gBaseGroundRowIdx + 1) == row))
    
    else if (gPipeRowIdx == row) {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                 Piper
        for (int i = 0; i < MAX_PIPE; i++) {
            if (gPipeColIdx[i] == col) {
                for (int k = 0; k < gPipeSize[i]; k++) {
                    m_BlockType[row - k][col].behaviour = (k == gPipeSize[i] - 1) ? PIPE : PIPE_BASE;
                
                    m_BlockType[row - k][col].size = gPipeSize[i] * BLOCK_SIZE;
                    SetObstaclePixels(row - k, col);
                    SetObstaclePixels(row - k, col + 1);
                }
                return;
            }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                 Steps
        for (int i = 0; i < MAX_STEP; i++) {
            if (gStepColIdx[i] == col) {
                for (int k = 0; k < gStepNum[i]; k++) {
                    m_BlockType[row - k][col].behaviour = STEP;
                    SetObstaclePixels(row - k, col);
                }
            }
        }
    } /// wlse if (gPipeRowIdx == row)
    
    else if (gRow5ForBlock == row) {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                 Blocks for Row 5
        for (int i = 0; i < MAX_BLOCKS; i ++) {
            if (col == gRow5ColIdx[i]) {
                m_BlockType[row][col].behaviour = gRow5Behaviour[i];
                m_BlockType[row][col].abilities = gRow5Ability[i];
                SetObstaclePixels(row, col);
                return;
            }
        }
    }
    
    else if (gRow9ForBlock == row) {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                 Blocks for Row 9
        for (int i = 0; i < MAX_BLOCKS; i ++) {
            if (col == gRow9ColIdx[i]) {
                m_BlockType[row][col].behaviour = gRow9Behaviour[i];
                m_BlockType[row][col].abilities = gRow9Ability[i];
                SetObstaclePixels(row, col);
                return;
            }
        }
    }
}

void Obstacle::InitObstacles() {
    for (int i = 0; i < NUM_ROW; i++) {
        for (int j = 0; j < NUM_COL; j++) {
            SetBlock(i, j);
        }
    }
//    for (int j = 0; j < 20; j++) {
//        m_BlockType[12][j].behaviour = BONUS;
//        SetObstaclePixels(12, j);
//    }
}


void Obstacle::ResetBlockPixels(const unsigned int row, const unsigned int col) {
    if ((row < NUM_ROW) && (col < NUM_COL)) {
        m_BlockType[row][col].bIsEmpty = false;
        m_BlockType[row][col].bIsObstacle = false;
//        m_BlockType[row][col].bIsPopped = false;
        m_BlockType[row][col].upPopped = 0;
        m_BlockType[row][col].size = BLOCK_SIZE;
        m_BlockType[row][col].behaviour = NO_BEHAV;
        m_BlockType[row][col].abilities = NO_ABILITY;
        
        float tileSize = (BLOCK_SIZE == GetBlockSizeAt(row, col) ? BLOCK_SIZE : BLOCK_SIZE << 1); //  TODO;
        float blockX = (col << BLOCK_SIZE_BIT);
        float blockY = (row << BLOCK_SIZE_BIT) + (BLOCK_SIZE >> 1);  /// Shift for lasg half ground block
        
        for (int i = blockY - BLOCK_SIZE; i < blockY; i++) {
            for (int j = blockX; j < blockX + tileSize; j++) {
                if (0 <= i) {
                    m_Obstacle[i][j].bIsObstacle = false;
                }
            }
        }
    }
    else {
        LogError (BIT_OBSTACLE, "Obstacle::ResetBlock(%d, %d) :  invalid row %d or col %d \n", NUM_ROW, NUM_COL, row, col);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Set Each Pixels of BLOCK as obstacle
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::SetObstaclePixels(const unsigned int row, const unsigned int col) {
    float tileSize = (BLOCK_SIZE == GetBlockSizeAt(row, col) ? BLOCK_SIZE : BLOCK_SIZE * 2); //  TODO;
    float blockX = (col << BLOCK_SIZE_BIT);
    float blockY = (row << BLOCK_SIZE_BIT) + (BLOCK_SIZE >> 1);  /// Shift for lasg half ground block
    
    for (int i = blockY - BLOCK_SIZE; i < blockY; i++) {
        for (int j = blockX; j < blockX + tileSize; j++) {
            if (0 <= i) {
                SetObstacle(i, j, GetBlockTypeAt(row, col), GetBlockAbilityAt(row, col));
            }
        }
    }
}


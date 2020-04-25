#include "Obstacle.h"
#include "Logger.h"

struct RowBlock_s {
    short colIdx;
    BlockType_e type;
    Ability_e ability;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Hollow base
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const short MAX_HOLLOW_BLOCK = 7;
const std::array<short, MAX_HOLLOW_BLOCK> gHollowColIdx {{69, 70, 86, 87, 88, 153, 154}};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pipes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const short MAX_PIPE = 6;
const short gPipeRowIdx = 12;
const std::array<short, MAX_PIPE> gPipeColIdx {{28, 38, 46, 57, 163, 179}};
const std::array<short, MAX_PIPE> gPipeSize {{2, 3, 4, 4, 2, 2}};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Steps
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const short MAX_STEP = 27;
const std::array<short, MAX_STEP> gNumStep {{
    1, 2, 3, 4,
    4, 3, 2, 1,
    1, 2, 3, 4, 4,
    4, 3, 2, 1, 1,
    2, 3, 4, 5, 6, 7, 8, 9, 1}};
const std::array<short, MAX_STEP> gStepColIdx {{
    134, 135, 136, 137,
    140, 141, 142, 143,
    148, 149, 150, 151, 152,
    155, 156, 157, 158,
    181, 182, 183, 184, 185, 186, 187, 188, 189, 198}};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Row 5 and 9 block
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const short MAX_BLOCKS = 21;
std::array<RowBlock_s, MAX_BLOCKS> gRow5blocks {{
    {22, BONUS, COIN_BONUS},
    {80, BRICKS, NONE}, {81, BRICKS, NONE}, {82, BRICKS, NONE}, {83, BRICKS, NONE}, {84, BRICKS, NONE}, {85, BRICKS, NONE}, {86, BRICKS, NONE}, {87, BRICKS, NONE},
    {91, BRICKS, NONE}, {92, BRICKS, NONE}, {93, BRICKS, NONE}, {94, BONUS, STAR_BONUS},
    {109, BONUS, MUSHROOM_BONUS},
    {121, BRICKS, NONE}, {122, BRICKS, NONE}, {123, BRICKS, NONE},
    {128, BRICKS, NONE}, {129, BONUS, COIN_BONUS}, {130, BONUS, COIN_BONUS }, {131, BRICKS, NONE}}};

const std::array<RowBlock_s, MAX_BLOCKS + 1> gRow9blocks {{
    {16, BONUS, COIN_BONUS},
    {20, BRICKS, NONE}, {21, BONUS, COIN_BONUS}, {22, BRICKS, NONE}, {23, BONUS, MUSHROOM_BONUS}, {24, BRICKS, NONE},
    {77, BRICKS, NONE}, {78, BONUS, MUSHROOM_BONUS}, {79, BRICKS, NONE},
    {94, BRICKS, NONE},
    {100, BRICKS, NONE}, {101, BRICKS, NONE},
    {106, BONUS, COIN_BONUS}, {109, BONUS, COIN_BONUS}, {112, BONUS, COIN_BONUS},
    {118, BRICKS, NONE},
    {129, BRICKS, NONE}, {130, BRICKS, NONE},
    {168, BRICKS, NONE}, {169, BRICKS, NONE}, {170, BONUS, COIN_BONUS}, {171, BRICKS, NONE}}};

const short gBaseGroundRowIdx = gPipeRowIdx + 1;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Set s specific Block
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::SetBlocks(Block &block) {
    if ((gBaseGroundRowIdx == block.m_RowNum) || ((gBaseGroundRowIdx + 1) == block.m_RowNum)){
        for (const auto &hollowCol : gHollowColIdx) {
            if (hollowCol == block.m_ColNum) {
                return;
            }
        }
        block.m_BlockType = BRICKS;
        block.m_Abilty = NONE;
        SetBlockPixels(block);
    } ///if (gBaseGroundRowIdx == block.m_RowNum)
    else if (gPipeRowIdx == block.m_RowNum) {
        static short pipeI = 0;
        static short stepI = 0;
        if (gPipeColIdx[pipeI] == block.m_ColNum) {
            for (int j = 0; j < gPipeSize[pipeI]; j++) {
                m_Block[block.m_RowNum - j][block.m_ColNum].m_BlockType = (j == gPipeSize[pipeI]) ? PIPE : PIPE_BASE;
                SetBlockPixels(m_Block[block.m_RowNum - j][block.m_ColNum]);
                SetBlockPixels(m_Block[block.m_RowNum - j][block.m_ColNum + 1]);
            }
            pipeI++;
        }
        else if (gStepColIdx[stepI] == block.m_ColNum) {
            for (int j = 0; j < gNumStep[stepI]; j++) {
                m_Block[block.m_RowNum - j][block.m_ColNum].m_BlockType = BLOCK;
                SetBlockPixels(m_Block[block.m_RowNum - j][block.m_ColNum]);
            }
            stepI++;
        }
    } /// else if (gPipeRowIdx == block.m_RowNum)
    else if (gRow5ForBlock == block.m_RowNum) {
        for (const auto &row5 : gRow5blocks) {
            if (block.m_ColNum == row5.colIdx) {
                block.m_BlockType = row5.type;
                block.m_Abilty = row5.ability;
                SetBlockPixels(block);
                return;
            }
        }
    }
    
    else if (gRow9ForBlock == block.m_RowNum) {
        for (const auto &row9 : gRow9blocks) {
            if (block.m_ColNum == row9.colIdx) {
                block.m_BlockType = row9.type;
                block.m_Abilty = row9.ability;
                SetBlockPixels(block);
                return;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Initialize the all blocks
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::InitObstacle() {
    for (auto &row : m_Block) {
        for (auto &col : row) {
            SetBlocks(col);
        }
    }
//    for (int j = 0; j < 6; j++) {
//        m_Block[9][3].m_BlockType = BONUS;
//        m_Block[9][3].m_Abilty = MUSHROOM_BONUS;
//
//        m_Block[5][3].m_BlockType = BONUS;
//        m_Block[5][3].m_Abilty = COIN_BONUS;
//
//        SetBlockPixels(m_Block[9][3]);
//        SetBlockPixels(m_Block[5][3]);
//    }
}

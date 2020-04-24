#include "Block.h"
#include "Logger.h"

unsigned short Block::m_NumInstance = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Block::Block()
: m_bIsPopped(false), m_bIsEmpty(false), m_UpPopped(0), m_RowNum(0), m_ColNum(0), m_BlockType(NO_TYPE), m_Abilty(EMPTY) {
    m_RowNum = m_NumInstance / NUM_COL;
    m_ColNum = m_NumInstance % NUM_COL;
    
    m_Position.X = m_ColNum << BLOCK_SIZE_BIT;
    m_Position.Y = (m_RowNum << BLOCK_SIZE_BIT) + (BLOCK_SIZE >> 1); /// row * 16 + 8 (Add 8 as last row is half in the mario background)
    
    m_NumInstance++;
//    LogInfo (BIT_BLOCK, "Block::Block(), Constructor Called !!! %d \n", m_NumInstance);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Block::~Block(){
//    LogInfo (BIT_BLOCK, "Block::~Block(), Destructor Called !!! \n");
}

#include "Obstacle.h"
#include "Logger.h"

Obstacle *Obstacle::m_Instance = nullptr;

const int gBlinkDelay = 10;
const int gMaxPopHeight = 16;   /// Maximum Height that block will move up when popped popped

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle::Obstacle() {
    for (auto &row : m_Obstacle) {
        row.fill(false);
    }
    InitObstacle();
    LogInfo(BIT_OBSTACLE, "Obstacle::Obstacle(), Constructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle::~Obstacle() {
    LogInfo(BIT_OBSTACLE, "Obstacle::~Obstacle(), Destructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Create a new instance if not created already and return
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle *Obstacle::GetInstance() {
    if (!m_Instance) {
        m_Instance = new Obstacle;
        LogInfo(BIT_OBSTACLE, "Obstacle::GetInstance(), Creating Obstacle Instance \n");
    }
    return m_Instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Delete instance if not deleted
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Obstacle *Obstacle::ReleaseInstance() {
    if (m_Instance) {
        delete m_Instance;
        m_Instance = nullptr;
        LogInfo(BIT_OBSTACLE, "Obstacle::ReleaseInstance(), Deleting Obstacle Instance \n");
    }
    return m_Instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Set all pixel of that block to true AS obstacle
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::SetBlockPixels(const Block &block) {
    for (int row = block.m_Position.Y; row > block.m_Position.Y - BLOCK_SIZE; row--) {
        for (int col = block.m_Position.X; col < block.m_Position.X + BLOCK_SIZE; col++) {
            m_Obstacle[row][col] = true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : ReSet all pixel of that block to true AS obstacle
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::ResetBlockPixel(Block &block) {
    block.m_BlockType = NO_TYPE;
//    block.m_Abilty = EMPTY;
//    block.m_bIsEmpty = false;
    
    for (int row = block.m_Position.Y; row > block.m_Position.Y - BLOCK_SIZE; row--) {
        for (int col = block.m_Position.X; col < block.m_Position.X + BLOCK_SIZE; col++) {
            m_Obstacle[row][col] = false;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Mark the block as empty
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::EmptyBlock(Block &block, short &imgIdxX) {
    /// Brick Broken
    if (BREAKABLE == block.m_Abilty) {
        ResetBlockPixel(block);
    }
    else {
        block.m_BlockType = BRICKS; /// as Empty Image is in Brick Row
        imgIdxX = 3;    /// Image of EMPTY block
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : pop up the block upon collsion
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::PopBlock(Block &block) {
    /// shift the block to height 8 once popped
    if (gMaxPopHeight > block.m_UpPopped++) {
        block.m_Position.Y--;
    }
    else {
        /// play the sound once
        if (COIN_BONUS == block.m_Abilty) {
            m_Sound.setBuffer(m_CoinSound);
        }
        else if ((BREAKABLE == block.m_Abilty)) {
            m_Sound.setBuffer(m_BrickSmashSound);
        }
        else if (MUSHROOM_BONUS == block.m_Abilty) {
            m_Sound.setBuffer(m_PowerUpAppearSound);
        }
        else if (NONE == block.m_Abilty) {
            m_Sound.setBuffer(m_BumpSound);
        }
        m_Sound.play();

        block.m_Position.Y += gMaxPopHeight;
        block.m_UpPopped = 0;
        
        if ((NONE != block.m_Abilty)) {
            block.m_bIsEmpty = true;
        }
        else {
            block.m_bIsPopped = false;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Blink the block
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::BlinkBlock(Block &block, short &imgIdxX) {
    if (BONUS == block.m_BlockType) {
        /// blicking of bonus
        static unsigned short blinkCounter = 0;
        imgIdxX = (blinkCounter++) >> gBlinkDelay;
        imgIdxX %= 4;  /// there are 4 images to blink
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Load and draw the Block Image
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::LoadBlockImage(sf::RenderWindow &window, const int row, const int col) {
    short imgIdxX = 0;
    Block &block = m_Block[row][col];
    
    if ((block.m_BlockType != BRICKS) && (block.m_BlockType != BONUS)) {
        return; /// We do not want to draw the Block
    }
    
    if (block.m_bIsEmpty) {
        EmptyBlock(block, imgIdxX);
    }
    else {
        if (EMPTY != block.m_Abilty) {
            (block.m_bIsPopped) ? PopBlock(block) : BlinkBlock(block, imgIdxX);
        }
    }
    
    TileMap::PrintControl_s printControl;
    printControl.imgType = TileMap::ITEM;
    printControl.tileSize = sf::Vector2<float>(BLOCK_SIZE, BLOCK_SIZE);
    printControl.xTileOffset = BLOCK_SIZE_BIT;
    printControl.imgXIdx = imgIdxX;
    printControl.imgYIdx = block.m_BlockType << BLOCK_SIZE_BIT;
    printControl.position = block.m_Position;

    m_Map.Load(printControl);
    window.draw(m_Map);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Set block as popped
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Obstacle::SetPoppedBlock(const int row, const int col, const bool bBigPlayer) {
    Block &block = m_Block[row][col];
    if (!block.m_bIsEmpty) {
        block.m_bIsPopped = true;
    }
    if ((bBigPlayer) && (NONE == block.m_Abilty)){
        m_Block[row][col].m_Abilty = BREAKABLE;
    }
}

#pragma once
#include <string>

const std::string gResourcePath = "/Users/ashish./Documents/XCode/MarioLevel1/MarioLevel1/Resources/";

const short BLOCK_SIZE = 16;
const short BLOCK_SIZE_BIT = 4;             /// as Block size is 16 so for divide and multiply operation use 4 to right or left shift

const short BLOCK_SIZE_SMALL = 8;
const short BLOCK_SIZE_SMALL_BIT = 3;       /// as Block size is 16 so for divide and multiply operation use 4 to right or left shift

/// Width we want se see in View at one time
const short VIEW_WIDTH = 256;
/// Actal height of Level 1 Image in pixels
const short VIEW_HEIGHT = 224;

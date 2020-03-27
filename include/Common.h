#pragma once
#include "Obstacle.h"

/// Path for Images used in Game
const std::string ResourcePath = "/Users/ashish./Documents/XCode/Mario/Mario/Resources/";

/// Window Size Factro From World View
const short WINDOW_WORLD_FACTOR = 4;

/// Mario World View size WORLD_VIEW_WIDTH X WORLD_VIEW_HEIGHT
const short WORLD_VIEW_HEIGHT = 224;
const short WORLD_VIEW_WIDTH = 256;

/// Window Pixels WIN_WIDTH X WIN_HEIGHT
const short WIN_HEIGHT = WORLD_VIEW_HEIGHT * 4;
const short WIN_WIDTH = WORLD_VIEW_WIDTH * 4;

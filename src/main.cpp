#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Logger.h"
#include "Mario.h"
#include <list>

int main(int argc, char const** argv) {
    Logger *loggerInstance = Logger::GetLoggerInstance();
    loggerInstance->SetLogLevel(LOG_DEBUG);
    loggerInstance->SetLogBitMask(0xff);

    {
        Mario mario;
        mario.PlayGame();
    }

    loggerInstance->ReleaseInstance();
    
    // 4 X 6
    
//    std::vector<std::vector<int>> lvec;
//    for (int i = 0; i < 6; i++) { ///  col
//        std::vector<int> colPixels;
//        for (int j = 0; j < 4; j++) { /// row
//            colPixels.push_back(1);
//        }
//        lvec.push_back(colPixels);
//    }
//
//    lvec.erase(lvec.begin());
//    std::vector<int> colPIx;
//    for (int j = 0; j < 4; j++) {
//        colPIx.push_back(0);
//    }
//    lvec.push_back(colPIx);
//
//    // row 3 col 4;
//    lvec[4][3] = 5;
//
//    for (auto i = 0; i < 4; i++) {
//        for (auto j = 0; j < 6; j++) {
//            std::cout << lvec[j][i];
//        }
//        std::cout << "\n";
//    }
    

    return EXIT_SUCCESS;
}
//30 11 23 83 64


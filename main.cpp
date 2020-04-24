#include <iostream>
#include "Logger.h"
#include "Mario.h"
#include "Timer.h"
#include <memory>

#include "Obstacle.h"

int main()
{
    Logger *loggerInstance = Logger::GetLoggerInstance();
    loggerInstance->SetLogLevel(Logger::LOG_DEBUG);
    loggerInstance->SetLogBitMask(0xffff);

//    Obstacle::GetInstance();
    {
//        WelcomeScreen welcome;
//        welcome.DrawWelcomeScreen();

        Mario mario;
        mario.PlayGame();
    }
    loggerInstance = Logger::ReleaseInstance();
    return EXIT_SUCCESS;
}

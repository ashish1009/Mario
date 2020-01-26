#include <iostream>
#include "Logger.h"
#include "Mario.h"

int main()
{
    Logger *loggerInstance = Logger::GetLoggerInstance();
    loggerInstance->SetLogLevel(LOG_DEBUG);
    loggerInstance->SetLogBitMask(0xff);
    
    {
        Mario mario;
        mario.PlayGame();
    }
    
    loggerInstance->ReleaseInstance();
    return 0;
}

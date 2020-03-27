#pragma once
#include <iostream>
#include <stdarg.h>
#include "FileBitMask.h"

/*
 Log Functions
 */
#define LogDebug(bitMask, ...) Logger::GetLoggerInstance()->LOG(LOG_DEBUG, bitMask, __VA_ARGS__)
#define LogInfo(bitMask, ...) Logger::GetLoggerInstance()->LOG(LOG_INFO, bitMask, __VA_ARGS__)
#define LogWarning(bitMask, ...) Logger::GetLoggerInstance()->LOG(LOG_WARNING, bitMask, __VA_ARGS__)
#define LogError(bitMask, ...) Logger::GetLoggerInstance()->LOG(LOG_ERROR, bitMask, __VA_ARGS__)

/*
 Log Level : defines the level of logger above which we all logs eill be printed
 */
enum LogLevel_e : unsigned char
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

/*
 Logger Class
 */
class Logger
{
private:
    static Logger *m_LoggerInstance;

private:
    LogLevel_e m_LogLevel;
    unsigned int m_LogBitMask;

private:
    Logger();

public:
    static Logger *GetLoggerInstance();
    static void ReleaseInstance();
    ~Logger();

public:
    void SetLogLevel(LogLevel_e level);
    void SetLogBitMask(const int mask);

    void LOG(LogLevel_e level, const int mask, const char *fmt, ...);
};

char *GetLogLevelString(LogLevel_e level);

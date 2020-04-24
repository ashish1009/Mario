#pragma once
#include <iostream>
#include <stdarg.h>
#include "FileBitMask.h"

/*
 Log Functions
 */
#define LogDebug(bitMask, ...) Logger::GetLoggerInstance()->LOG(Logger::LOG_DEBUG, bitMask, __VA_ARGS__)
#define LogInfo(bitMask, ...) Logger::GetLoggerInstance()->LOG(Logger::LOG_INFO, bitMask, __VA_ARGS__)
#define LogWarning(bitMask, ...) Logger::GetLoggerInstance()->LOG(Logger::LOG_WARNING, bitMask, __VA_ARGS__)
#define LogError(bitMask, ...) Logger::GetLoggerInstance()->LOG(Logger::LOG_ERROR, bitMask, __VA_ARGS__)

/*
 Logger Class
 */
class Logger
{
public:
    /// Log Level : defines the level of logger above which we all logs eill be printed
    enum LogLevel_e : unsigned char
    {
        LOG_DEBUG = 0,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };
    
private:
    static Logger *m_Instance;

    LogLevel_e m_LogLevel;
    unsigned int m_LogBitMask;

private:
    Logger();

public:
    static Logger *GetLoggerInstance();
    static Logger *ReleaseInstance();
    ~Logger();

    void SetLogLevel(const LogLevel_e level);
    void SetLogBitMask(const int mask);

    void LOG(LogLevel_e level, const int mask, const char *fmt, ...);
};

const char *const GetLogLevelString(const Logger::LogLevel_e level);

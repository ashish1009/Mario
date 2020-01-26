#include <Logger.h>

char *GetLogLevelString(LogLevel_e level)
{
    switch (level)
    {
    case LOG_ERROR:
        return (char *)"ERROR ";
        break;

    case LOG_WARNING:
        return (char *)"WARNING ";
        break;

    case LOG_INFO:
        return (char *)"INFO ";
        break;

    case LOG_DEBUG:
        return (char *)"DEBUG ";
        break;

    default:
        break;
    }
}

Logger *Logger::m_LoggerInstance = nullptr;

Logger::Logger()
    : m_LogLevel(LOG_ERROR), m_LogBitMask(0)
{
    std::cout << " [LOGGER] Logger::Logger(), Logger Constructor called with level " << m_LogLevel << " : " << GetLogLevelString(m_LogLevel) << " Bit Mask " << std::hex << m_LogBitMask << std::endl;
}

Logger::~Logger()
{
    std::cout << " [LOGGER] Logger::~Logger(), Logger Destructor called " << std::endl;
}

Logger *Logger::GetLoggerInstance()
{
    if (nullptr == m_LoggerInstance)
    {
        std::cout << " [LOGGER] Logger::GetLoggerInstance, Creating Logger Instance " << std::endl;
        m_LoggerInstance = new Logger;
    }
    return m_LoggerInstance;
}

void Logger::ReleaseInstance()
{
    if (nullptr != m_LoggerInstance)
    {
        std::cout << " [LOGGER] Logger::ReleaseInstance, Deleting Logger Instance " << std::endl;
        delete m_LoggerInstance;
    }
}

void Logger::SetLogLevel(LogLevel_e level)
{
    if ((LOG_DEBUG <= level) && (LOG_ERROR >= level))
    {
        std::cout << " [LOGGER] Logger::SetLogLevel(), Log level Set to " << level << std::endl;
        m_LogLevel = level;
    }
    else
    {
        std::cout << "[LOGGER : ERROR] : Logger::SetLogLevel(), level " << level << " is out of range { " << LOG_DEBUG << ", " << LOG_ERROR << " }" << std::endl;
    }
}

void Logger::LOG(LogLevel_e level, const unsigned int mask, const char *fmt, ...)
{
    if ((m_LogLevel <= level) && ((m_LogBitMask >> mask) & 0x01))
    {
        std::cout << "[ " << GetLogLevelString(level) << " ] ";

        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

void Logger::SetLogBitMask(const unsigned int mask)
{
    m_LogBitMask = mask;
    std::cout << " [LOGGER] Logger::SetLogBitMask(), Bit Mask set to : " << std::hex << m_LogBitMask << std::endl;
}

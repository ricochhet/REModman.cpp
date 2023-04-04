#ifndef REMODMAN_LOGGER_INCLUDED
#define REMODMAN_LOGGER_INCLUDED

#pragma once

#include <string>
#include <iostream>

enum class LogLevel
{
    OK,
    Debug,
    Info,
    Warning,
    Error
};

class Logger
{
public:
    static Logger &getInstance();
    void setLogLevel(LogLevel level);
    int getColorCode(LogLevel level);
    void log(const std::string &message, LogLevel level = LogLevel::Info);

private:
    Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    LogLevel m_logLevel;
};

#endif // REMODMAN_LOGGER_INCLUDED
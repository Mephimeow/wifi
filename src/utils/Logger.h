#pragma once
#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {
public:
    static Logger& instance();

    void setLevel(LogLevel level);
    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);

private:
    Logger();
    void log(LogLevel level, const std::string& msg);
    std::string levelToString(LogLevel level);

    LogLevel minLevel = LogLevel::INFO;
    std::mutex mutex;
};

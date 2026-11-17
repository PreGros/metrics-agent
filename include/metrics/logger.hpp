#pragma once
#include <mutex>
#include <ostream>
#include <string_view>

namespace metrics {

enum class LogLevel { Debug, Info, Warn, Error };

class Logger {
private:
    std::ostream& out_;
    LogLevel min_level_;
    std::mutex mutex_;
public:
    explicit Logger(std::ostream& out, LogLevel min_level = LogLevel::Info);

    void set_level(LogLevel level);
    void log(LogLevel level, std::string_view message);


};

}  // namespace metrics
#include "metrics/logger.hpp"

namespace metrics {

namespace {
const char* prefix(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return "[DEBUG] ";
        case LogLevel::Info:  return "[INFO] ";
        case LogLevel::Warn:  return "[WARN] ";
        case LogLevel::Error: return "[ERROR] ";
    }
    return "[?] ";
}
}  // namespace

Logger::Logger(std::ostream& out, LogLevel min_level)
    : out_(out), min_level_(min_level) {}

void Logger::set_level(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    min_level_ = level;
}

void Logger::log(LogLevel level, std::string_view message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (level < min_level_) return;
    out_ << prefix(level) << message << '\n';
}

}  // namespace metrics
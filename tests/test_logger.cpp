#include <gtest/gtest.h>
#include <sstream>
#include "metrics/logger.hpp"

using namespace metrics;

TEST(Logger, FiltersMessagesBelowMinLevel) {
    std::ostringstream out;
    Logger log(out, LogLevel::Warn);

    log.log(LogLevel::Info, "hidden");
    log.log(LogLevel::Error, "shown");

    EXPECT_EQ(out.str(), "[ERROR] shown\n");
}

TEST(Logger, SetLevelChangesFilteringAtRuntime) {
    std::ostringstream out;

    Logger log(out, LogLevel::Debug);
    log.log(LogLevel::Info, "before");

    log.set_level(LogLevel::Error);
    log.log(LogLevel::Info, "after");

    EXPECT_EQ(out.str(), "[INFO] before\n");
}

TEST(Logger, TestPrefixes) {
    std::ostringstream out;

    Logger log(out, LogLevel::Debug);
    log.log(LogLevel::Info, "Information!");
    log.log(LogLevel::Debug, "Debugging!");
    log.log(LogLevel::Warn, "Warning!");
    log.log(LogLevel::Error, "Erroring!");

    EXPECT_EQ(out.str(), "[INFO] Information!\n[DEBUG] Debugging!\n[WARN] Warning!\n[ERROR] Erroring!\n");
}
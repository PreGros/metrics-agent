#include "metrics/logger.hpp"
#include <iostream>

int main() {
    metrics::Logger logger(std::cout);

    logger.log(metrics::LogLevel::Info, "Metrics agent started");
    logger.log(metrics::LogLevel::Warn, "Test warning");

    return 0;
}
#include <gtest/gtest.h>
#include <sstream>
#include "metrics/ring_buffer.hpp"

using namespace metrics;

TEST(RingBuffer, EmptyBuffer) {
    RingBuffer<int> ringBuffer(3);

    EXPECT_TRUE(ringBuffer.empty());
    EXPECT_EQ(ringBuffer.size(), 0);
    EXPECT_TRUE(ringBuffer.snapshot().empty());
}

TEST(RingBuffer, PushFewerValuesThanCapacity) {
    RingBuffer<int> ringBuffer(3);

    for (size_t i = 0; i < 2; i++) {
        ringBuffer.push(i);
    }
    
    EXPECT_EQ(ringBuffer.size(), 2);
    EXPECT_EQ(ringBuffer.snapshot(), (std::vector<int>{0, 1}));
}

TEST(RingBuffer, ReplaceValueOverCapacity) {
    RingBuffer<int> ringBuffer(3);

    for (size_t i = 0; i < 4; i++) {
        ringBuffer.push(i);
    }
    
    std::vector<int> values = ringBuffer.snapshot();

    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values[1], 2);
    EXPECT_EQ(values[2], 3);
}

TEST(RingBuffer, ZeroCapacityThrows) {
    EXPECT_THROW(RingBuffer<int>(0), std::invalid_argument);
}

TEST(RingBuffer, SnapshotCorrectAfterClearOnWrappedBuffer) {
    RingBuffer<int> rb(3);
    for (int i = 1; i <= 4; i++) rb.push(i);
    rb.clear();
    rb.push(9);

    EXPECT_EQ(rb.snapshot(), (std::vector<int>{9}));
}

TEST(RingBuffer, CapacityOne) {
    RingBuffer<int> ringBuffer(1);

    ringBuffer.push(1);
    ringBuffer.push(2);
    ringBuffer.push(3);

    EXPECT_EQ(ringBuffer.size(), 1);
    EXPECT_EQ(ringBuffer.snapshot(), (std::vector<int>{3}));
}

TEST(RingBuffer, MultipleWrapArounds) {
    RingBuffer<int> ringBuffer(3);

    for (int i = 1; i <= 10; i++) {
        ringBuffer.push(i);
    }

    EXPECT_EQ(ringBuffer.size(), 3);
    EXPECT_EQ(ringBuffer.snapshot(), (std::vector<int>{8, 9, 10}));
}

TEST(RingBuffer, StringValues) {
    RingBuffer<std::string> ringBuffer(3);

    std::string first = "Hello";
    std::string second = "World";

    ringBuffer.push(first);
    ringBuffer.push(second);

    first = "Changed";
    second = "Modified";

    EXPECT_EQ(ringBuffer.size(), 2);
    EXPECT_EQ(
        ringBuffer.snapshot(),
        (std::vector<std::string>{"Hello", "World"})
    );
}

TEST(RingBuffer, CapacityRemainsConstant) {
    RingBuffer<int> ringBuffer(3);

    EXPECT_EQ(ringBuffer.capacity(), 3);

    for (int i = 1; i <= 10; i++) {
        ringBuffer.push(i);
    }

    EXPECT_EQ(ringBuffer.capacity(), 3);
    EXPECT_EQ(ringBuffer.size(), 3);
}
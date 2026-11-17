#pragma once
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace metrics {

// Not thread-safe: external synchronization is required.
template <typename T>
class RingBuffer {
private:
    std::vector<T> data_;
    std::size_t head_ = 0;
    std::size_t count_ = 0;
    
public:
    explicit RingBuffer(std::size_t capacity);

    void push(const T& value);
    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;
    std::vector<T> snapshot() const;
    void clear();
};

namespace detail {

inline std::size_t validate_capacity(std::size_t capacity) {
    if (capacity == 0) {
        throw std::invalid_argument("Capacity must be greater than 0");
    }

    return capacity;
}

} // namespace detail

template <typename T>
RingBuffer<T>::RingBuffer(std::size_t capacity)
    : data_(detail::validate_capacity(capacity)) {}

template <typename T>
void RingBuffer<T>::push(const T& value) {
    data_[head_] = value;
    head_ = (head_ + 1) % data_.size();
    if (count_ < data_.size()) {
        count_++;
    }
}

template <typename T>
std::size_t RingBuffer<T>::size() const {
    return count_;
};

template <typename T>
std::size_t RingBuffer<T>::capacity() const {
    return data_.size();
}

template <typename T>
bool RingBuffer<T>::empty() const {
    return count_ == 0;
}

template <typename T>
std::vector<T> RingBuffer<T>::snapshot() const {
    std::vector<T> result;
    result.reserve(count_);

    const std::size_t cap = data_.size();
    const std::size_t start = (head_ + cap - count_) % cap;

    for (std::size_t i = 0; i < count_; ++i) {
        result.push_back(data_[(start + i) % cap]);
    }
    return result;
}

template <typename T>
void RingBuffer<T>::clear() {
    head_ = 0;
    count_ = 0;
}

} // namespace metrics
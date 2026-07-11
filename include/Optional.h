#pragma once

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value) : hasValue_(true), value_(value) {}

    explicit operator bool() const { return hasValue_; }
    const T& operator*() const { return value_; }
    T& operator*() { return value_; }
    const T* operator->() const { return &value_; }
    T* operator->() { return &value_; }

private:
    bool hasValue_ = false;
    T value_{};
};

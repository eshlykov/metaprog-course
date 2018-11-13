#pragma once

template <typename T>
class IAllocator {
public:
    virtual ~IAllocator() = default;

    virtual T* Allocate() = 0;

public:
    using TType = T;
};

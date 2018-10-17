#pragma once

#include "observer.hpp"
#include <memory>

class IListener;

class IObservable {
protected:
    std::shared_ptr<TObserver> observer{new TObserver{}};

public:
    virtual ~IObservable() = default;

    virtual void AddListener(std::shared_ptr<IListener>) = 0;

    virtual void RemoveListener(std::shared_ptr<IListener>) = 0;
};

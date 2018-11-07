#pragma once

#include <memory>

class IObservable;

class IListener {
public:
    virtual ~IListener() = default;

    virtual void Subscribe(std::shared_ptr<IObservable>) = 0;

    virtual void Unsubscribe(std::shared_ptr<IObservable>) = 0;

    virtual void Notify(const std::string&) = 0;
};

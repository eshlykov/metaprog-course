#pragma once

#include "mediator.hpp"
#include "observable.hpp"
#include "permissions.hpp"
#include "server.hpp"
#include <memory>
#include <string>

class TLoggable : public IServer, public IObservable, public std::enable_shared_from_this<TLoggable> {
public:
    void ConnectToMediator(std::shared_ptr<TMediator> mediator) {
        if (mediator == nullptr) {
            throw std::logic_error("Pointer to mediator cannot be null.");
        }
        mediator->AddServer(std::dynamic_pointer_cast<IServer>(shared_from_this()));
    }

    virtual std::string GetData(EPermissions) const override {
        return data;
    }

    virtual void UpdateData(EPermissions permissions, const std::string& _data) override {
        data = _data;
        observer->NotifyAll(GetData(permissions));
    }

    virtual void AddListener(std::shared_ptr<IListener> listener) override {
        observer->AddListener(listener);
    }

    virtual void RemoveListener(std::shared_ptr<IListener> listener) override {
        observer->RemoveListener(listener);
    }
};

#pragma once

#include "client.hpp"
#include "listener.hpp"
#include "mediator.hpp"
#include "observable.hpp"
#include "permissions.hpp"
#include <algorithm>
#include <memory>
#include <vector>

class TLogger : public IClient, public IListener, public std::enable_shared_from_this<TLogger> {
public:
    TLogger() = default;

    explicit TLogger(EPermissions _permissions) {
        permissions = _permissions;
    }

    explicit TLogger(std::shared_ptr<TMediator> _mediator) {
        mediator = _mediator;
    }

    virtual void LogData(const std::string& data) override {
        log.emplace_back(data);
    }

    virtual void LogDataOf(std::shared_ptr<IServer> server) override {
        LogData(server->GetData(permissions));
    }

    virtual void Subscribe(std::shared_ptr<IObservable> observerable) override {
        observerable->AddListener(std::dynamic_pointer_cast<IListener>(shared_from_this()));
    }

    virtual void Unsubscribe(std::shared_ptr<IObservable> observerable) override {
        observerable->RemoveListener(std::dynamic_pointer_cast<IListener>(shared_from_this()));
    }

    virtual void Notify(const std::string& data) override {
        LogData(data);
    }

    virtual EPermissions GetPermissions() const override {
        return permissions;
    }

    bool Has(const std::string& record) {
        return std::find(log.begin(), log.end(), record) != log.end();
    }
};

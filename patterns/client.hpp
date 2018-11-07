#pragma once

#include "pattern.hpp"
#include "permissions.hpp"
#include <memory>
#include <string>
#include <vector>

class IServer;

class IClient {
protected:
    std::shared_ptr<TMediator> mediator = nullptr;
    EPermissions permissions = EPermissions::PNone;
    std::vector<std::string> log = {};

public:
    virtual ~IClient() = default;

    virtual void LogDataOf(std::shared_ptr<IServer>) = 0;

    virtual void LogData(const std::string&) = 0;

    virtual EPermissions GetPermissions() const = 0;
};

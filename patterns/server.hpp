#pragma once

#include "pattern.hpp"
#include "permissions.hpp"
#include <memory>
#include <string>

class IClient;

class IServer {
protected:
    std::string data = {};

public:
    virtual ~IServer() = default;

    virtual std::string GetData(EPermissions) const = 0;

    virtual void UpdateData(EPermissions, const std::string&) = 0;
};

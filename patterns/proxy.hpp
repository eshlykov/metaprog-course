#pragma once

#include "pattern.hpp"
#include "permissions.hpp"
#include "server.hpp"
#include <memory>

template <>
class TPattern<EPatternKind::PkProxy> : public IServer {
private:
    std::shared_ptr<IServer> server = nullptr;

public:
    explicit TPattern(std::shared_ptr<IServer> _server) :
        server{_server} {
    }

    virtual std::string GetData(EPermissions permissions) const override {
        if (permissions == EPermissions::PRead || permissions == EPermissions::PWrite) {
            return server->GetData(permissions);
        } else {
            throw std::logic_error("Permission denied.");
        }
    }

    virtual void UpdateData(EPermissions permissions, const std::string& data) override {
        if (permissions == EPermissions::PWrite) {
            server->UpdateData(permissions, data);
        } else {
            throw std::logic_error("Permission denied.");
        }
    }
};

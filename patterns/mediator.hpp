#pragma once

#include "client.hpp"
#include "pattern.hpp"
#include "server.hpp"
#include <memory>
#include <unordered_set>
#include <vector>

template <>
class TPattern<EPatternKind::PkMediator> {
private:
    std::unordered_set<std::shared_ptr<IServer>> servers = {};

public:
    void AddServer(std::shared_ptr<IServer> server) {
        servers.insert(server);
    }

    void LogAllDataTo(std::shared_ptr<IClient> client) const {
        for (auto&& server : servers) {
            client->LogData(server->GetData(client->GetPermissions()));
        }
    }
};

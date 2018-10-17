#pragma once

#include "listener.hpp"
#include "pattern.hpp"
#include <memory>
#include <unordered_set>

template <>
class TPattern<EPatternKind::PkObserver> {
private:
    std::unordered_set<std::shared_ptr<IListener>> listeners = {};

public:
    void AddListener(std::shared_ptr<IListener> listener) {
        listeners.insert(listener);
    }

    void RemoveListener(std::shared_ptr<IListener> listener) {
        listeners.erase(listener);
    }

    void NotifyAll(const std::string& data) {
        for (auto&& listener : listeners) {
            listener->Notify(data);
        }
    }
};

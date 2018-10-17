#include "loggable.hpp"
#include "logger.hpp"
#include "mediator.hpp"
#include "permissions.hpp"
#include "proxy.hpp"
#include <cassert>
#include <memory>

template <typename F>
void Try(F&& action) {
    try {
        action();
    } catch (...) {
    }
}

void TestProxy() {
    std::shared_ptr<TLoggable> loggable{new TLoggable{}};
    std::shared_ptr<TProxy> proxy{new TProxy{loggable}};

    proxy->UpdateData(EPermissions::PWrite, "1");

    std::shared_ptr<TLogger> clientNone{new TLogger{EPermissions::PNone}};
    std::shared_ptr<TLogger> clientRead{new TLogger{EPermissions::PRead}};
    std::shared_ptr<TLogger> clientWrite{new TLogger{EPermissions::PWrite}};

    Try([&] { clientNone->LogDataOf(proxy); });
    Try([&] { clientRead->LogDataOf(proxy); });
    Try([&] { clientWrite->LogDataOf(proxy); });

    assert(!clientNone->Has("1"));
    assert(clientRead->Has("1"));
    assert(clientWrite->Has("1"));
}

void TestObloggable() {
    std::shared_ptr<TLogger> logger{new TLogger{}};

    std::shared_ptr<TLoggable> loggable1{new TLoggable{}};
    std::shared_ptr<TLoggable> loggable2{new TLoggable{}};

    loggable1->UpdateData(EPermissions::PWrite, "1");
    assert(!logger->Has("1"));

    logger->Subscribe(loggable1);
    loggable1->UpdateData(EPermissions::PWrite, "2");
    assert(logger->Has("2"));

    loggable2->UpdateData(EPermissions::PWrite, "3");
    assert(!logger->Has("3"));

    logger->Subscribe(loggable2);
    loggable1->UpdateData(EPermissions::PWrite, "4");
    loggable2->UpdateData(EPermissions::PWrite, "5");
    assert(logger->Has("4"));
    assert(logger->Has("5"));

    logger->Unsubscribe(loggable1);
    loggable1->UpdateData(EPermissions::PWrite, "6");
    assert(!logger->Has("6"));
}

void TestMediator() {
    std::shared_ptr<TMediator> mediator{new TMediator{}};

    std::shared_ptr<TLogger> logger{new TLogger{mediator}};

    std::shared_ptr<TLoggable> loggable1{new TLoggable{}};
    std::shared_ptr<TLoggable> loggable2{new TLoggable{}};

    loggable1->ConnectToMediator(mediator);
    loggable2->ConnectToMediator(mediator);

    loggable1->UpdateData(EPermissions::PWrite, "1");
    loggable2->UpdateData(EPermissions::PWrite, "2");

    assert(!logger->Has("1"));
    assert(!logger->Has("2"));

    mediator->LogAllDataTo(logger);

    assert(logger->Has("1"));
    assert(logger->Has("2"));
}

int main() {
    TestProxy();
    TestObloggable();
    TestMediator();
}

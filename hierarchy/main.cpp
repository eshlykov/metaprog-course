#include "variant.hpp"
#include "event-handler.hpp"
#include <cassert>
#include <iostream>

class TClearingHandler : public TLinearHierarchy<TEventHandler, TTypeList<int, double, bool>> {
public:
    virtual void HandleEvent(int& object, EEventType eventType) override final {
        DoHandleEvent(object, eventType);
    }

    virtual void HandleEvent(double& object, EEventType eventType) override final {
        DoHandleEvent(object, eventType);
    }

    virtual void HandleEvent(bool& object, EEventType eventType) override final {
        DoHandleEvent(object, eventType);
    }

private:
    template <typename TObjectType>
    void DoHandleEvent(TObjectType& object, EEventType eventType) {
        if (eventType == EEventType::EtClear) {
            object = {};
        }
    }
};

int main() {
    TVariant<bool, int, double> variant;

    variant.Set<int>(4);
    variant.Set<bool>(true);
    variant.Set<double>(-2.2);

    assert(variant.Get<int>() == 4);
    assert(variant.Get<bool>() == true);
    assert(variant.Get<double>() == -2.2);

    TClearingHandler clearer = {};

    int temp1 = 1;
    double temp2 = 2;
    bool temp3 = true;

    clearer.HandleEvent(temp1, EEventType::EtClear);
    clearer.HandleEvent(temp2, EEventType::EtClear);
    clearer.HandleEvent(temp3, EEventType::EtClear);

    assert(temp1 == 0);
    assert(temp2 == 0);
    assert(temp3 == false);
}

#pragma once

#include "../hierarchy/scattered-hierarchy.hpp"
#include "metacurrying.hpp"
#include "type-holder.hpp"
#include <memory>

template <typename TUnit>
class IAbstractFactoryUnit {
public:
    virtual ~IAbstractFactoryUnit() = default;

    template <typename T>
    std::unique_ptr<T> CreateUnit() {
        return DoCreateUnit(TTypeHolder<T>{});
    }

private:
    virtual std::unique_ptr<TUnit> DoCreateUnit(TTypeHolder<TUnit>) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
class IAbstractFactory : public TScatteredHierarchy<IAbstractFactoryUnit, TList> {
public:
    virtual ~IAbstractFactory() = default;

    template <typename TUnit>
    std::unique_ptr<TUnit> CreateUnit() {
        return GetAbstractFactoryUnit<TUnit>().template CreateUnit<TUnit>();
    }

private:
    template <typename TUnit>
    IAbstractFactoryUnit<TUnit>& GetAbstractFactoryUnit() {
        static_assert(THas<TList, TUnit>::value, "Type is not supported");
        return static_cast<IAbstractFactoryUnit<TUnit>&>(*this);
    }
};

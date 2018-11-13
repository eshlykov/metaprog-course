#pragma once

#include "../hierarchy/scattered-hierarchy.hpp"
#include "metacurrying.hpp"
#include "type-holder.hpp"
#include <memory>

template <typename TUnit, typename TAllocator>
class IAbstractFactoryUnit {
    static_assert(std::is_base_of_v<typename TAllocator::TType, TUnit>, "Allocator does not support type used");
public:
    virtual ~IAbstractFactoryUnit() = default;

    std::unique_ptr<TUnit> CreateUnit(TTypeHolder<TUnit>) {
        auto* unit = static_cast<TUnit*>(allocator.Allocate());
        DoCreateUnit(unit);
        return std::unique_ptr<TUnit>{unit};
    };

private:
    virtual void DoCreateUnit(TUnit*) = 0;

private:
    TAllocator allocator = {};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename TAllocator>
class IAbstractFactory : public TScatteredHierarchy<TMetacurrying<IAbstractFactoryUnit, TAllocator>::template TCurriedType, TList> {
public:
    virtual ~IAbstractFactory() = default;

    template <typename TUnit>
    std::unique_ptr<TUnit> CreateUnit() {
        return GetAbstractFactoryUnit<TUnit>().CreateUnit(TTypeHolder<TUnit>{});
    }

private:
    template <typename TUnit>
    IAbstractFactoryUnit<TUnit, TAllocator>& GetAbstractFactoryUnit() {
        static_assert(THas<TList, TUnit>::value, "Type is not supported");
        return static_cast<IAbstractFactoryUnit<TUnit, TAllocator>&>(*this);
    }
};

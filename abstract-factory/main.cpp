#include "../type-list/type-list.hpp"
#include "../hierarchy/linear-hierarchy.hpp"
#include "allocator.hpp"
#include "abstract-factory.hpp"
#include <cstdlib>
#include <memory>

class TEnemy {
};

class TInfantry : public TEnemy {
};

class TArcher : public TEnemy {
};

class TCavalry : public TEnemy {
};

class TRomanInfantry : public TInfantry {
};

class TCarthInfantry : public TInfantry {
};

class TRomanArcher : public TArcher {
};

class TCarthArcher : public TArcher {
};

class TRomanCavalry : public TCavalry {
};

class TCarthCavalry : public TCavalry {
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class TAllocatorNew : public IAllocator<T> {
public:
    virtual ~TAllocatorNew() = default;

    virtual T* Allocate() override final {
        return new T{};
    }
};

template <typename T>
class TAllocatorMalloc : public IAllocator<T> {
public:
    virtual ~TAllocatorMalloc() = default;

    virtual T* Allocate() override final {
        auto* buffer = static_cast<T*>(std::malloc(sizeof(T)));
        return new(buffer) T{};
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename TUnit, typename TBase, typename TAllocator>
class TFactorUnit : public TBase {
    static_assert(std::is_base_of_v<typename TAllocator::TType, TUnit>, "Allocator does not support type used");
public:
    virtual ~TFactorUnit() = default;

private:
    virtual std::unique_ptr<TUnit> DoCreateUnit(TTypeHolder<TUnit>) override {
        auto* unit = static_cast<TUnit*>(allocator.Allocate());
        return std::unique_ptr<TUnit>{unit};
    }

private:
    TAllocator allocator = {};
};

class IEnemyFactory : public IAbstractFactory<TTypeList<TInfantry, TArcher, TCavalry>> {
public:
    virtual ~IEnemyFactory() = default;
};

template <template <typename> typename TAllocator>
class TEnemyFactory : public TLinearHierarchy<
    TMetacurrying<TFactorUnit, TAllocator<TEnemy>>::template TCurriedType,
    TTypeList<TInfantry, TArcher, TCavalry>,
    IEnemyFactory> {
public:
    virtual ~TEnemyFactory() = default;
};

int main() {
    TEnemyFactory<TAllocatorNew> newUnitFactory = {};
    TEnemyFactory<TAllocatorMalloc> mallocUnitFactory = {};

    newUnitFactory.CreateUnit<TInfantry>();
    mallocUnitFactory.CreateUnit<TArcher>();
}

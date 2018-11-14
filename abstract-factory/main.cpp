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


template <typename TUnit, typename TBase>
class TFactorUnit : public TBase {
public:
    virtual ~TFactorUnit() = default;

    virtual void DoCreateUnit(TUnit*) override {
    }
};

template <template <typename> typename TAllocator>
class IEnemyFactory : public IAbstractFactory<TTypeList<TInfantry, TArcher, TCavalry>, TAllocator<TEnemy>> {
public:
    virtual ~IEnemyFactory() = default;
};

template <template <typename> typename TAllocator>
class TEnemyFactory : public TLinearHierarchy<TFactorUnit, TTypeList<TInfantry, TArcher, TCavalry>, IEnemyFactory<TAllocator>> {
public:
    virtual ~TEnemyFactory() = default;
};

int main() {
    TEnemyFactory<TAllocatorNew> newUnitFactory = {};
    TEnemyFactory<TAllocatorMalloc> mallocUnitFactory = {};

    newUnitFactory.CreateUnit<TArcher>();
    mallocUnitFactory.CreateUnit<TCavalry>();
}

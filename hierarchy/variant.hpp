#pragma once

#include "../type-list/type-list.hpp"
#include "scatter-hierarchy.hpp"
#include <stdexcept>
#include <utility>

template <typename T>
class THolder {
public:
    T Value = {};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename... TTypes>
class TVariant : public TScatteredHierarchy<THolder, TTypeList<TTypes...>> {
public:
    template <typename T>
    T Get() {
        return GetVariant<T>().Value;
    }

    template <typename T>
    void Set(T&& value) {
        GetVariant<T>().Value = std::forward<T>(value);
    }

private:
    template <typename T>
    THolder<T>& GetVariant() {
        static_assert(THas<TTypeList<TTypes...>, T>::value, "Type is not supported");
        return static_cast<THolder<T>&>(*this);
    }
};

#pragma once

#include "../type-list/type-list.hpp"

template <template <typename...> typename, typename>
class TScatteredHierarchy;

template <template <typename...> typename TUnit, typename... TTypes>
class TScatteredHierarchy<TUnit, TTypeList<TTypes...>> : public TUnit<TTypes>... {
public:
    using TBases = TTypeList<TUnit<TTypes>...>;
};

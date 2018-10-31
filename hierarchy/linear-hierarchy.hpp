#pragma once

#include "../type-list/type-list.hpp"

template <template <typename, typename> typename TUnit, typename TList>
class TLinearHierarchy : public TUnit<THead<TList>, TLinearHierarchy<TUnit, typename TList::TTail>> {
};

template <template <typename, typename> typename TUnit>
class TLinearHierarchy<TUnit, TTypeList<>> {
};

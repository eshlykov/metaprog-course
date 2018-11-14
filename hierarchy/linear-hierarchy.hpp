#pragma once

#include "../type-list/type-list.hpp"

template <template <typename, typename> typename TUnit, typename TList, typename TBase = TTypeList<>>
class TLinearHierarchy : public TUnit<THead<TList>, TLinearHierarchy<TUnit, typename TList::TTail, TBase>> {
};

template <template <typename, typename> typename TUnit, typename TBase>
class TLinearHierarchy<TUnit, TTypeList<>, TBase> : public TBase {
};

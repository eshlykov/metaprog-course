#pragma once

#include "../hierarchy/linear-hierarchy.hpp"
#include "../hierarchy/scattered-hierarchy.hpp"
#include "../type-list/type-list.hpp"

template <template <typename, typename> typename, typename, typename>
class TLinearHierarchyMapper;

template <template <typename, typename> typename TUnit, typename TRoot, typename... TTypeLists>
class TLinearHierarchyMapper<TUnit, TRoot, TTypeList<TTypeLists...>> {
public:
    using TValue = TTypeList<TLinearHierarchy<TUnit, TTypeLists, TRoot>...>;
};

template <
    typename TList,
    template <typename> typename TBranchHead,
    template <typename, typename> typename TBranchNode,
    typename TBranchTail,
    template <typename, typename> typename TBranchCondition>
using TTreeHierarchy = TScatteredHierarchy<
    TBranchHead,
    typename TLinearHierarchyMapper<TBranchNode, TBranchTail, TGroupBy<TList, TBranchCondition>>::TValue>;

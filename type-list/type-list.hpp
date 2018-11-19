#pragma once

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename...>
class TTypeList;

template <>
class TTypeList<> {
public:
    using THead = TTypeList<>;
    using TTail = TTypeList<>;

    static constexpr int Length = 0;
};

template <typename TType, typename... TTypes>
class TTypeList<TType, TTypes...> {
public:
    using THead = TType;
    using TTail = TTypeList<TTypes...>;

    static constexpr int Length = 1 + TTail::Length;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
using TLength = std::integral_constant<int, TList::Length>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int index>
class TAtHolder {
public:
    static_assert(index > 0 && index < TList::Length, "Index out of range.");

    using TValue = typename TAtHolder<typename TList::TTail, index - 1>::TValue;
};

template <typename TList>
class TAtHolder<TList, 0> {
public:
    static_assert(TList::Length > 0, "Index out of range.");

    using TValue = typename TList::THead;
};


template <typename TList, int index>
using TAt = typename TAtHolder<TList, index>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename TType>
class THasHolder {
public:
    using TValue = std::disjunction<
        std::is_same<typename TList::THead, TType>,
        typename THasHolder<typename TList::TTail, TType>::TValue>;
};

template <typename TType>
class THasHolder<TTypeList<>, TType> {
public:
    using TValue = std::false_type;
};

template <typename TList, typename TType>
using THas = typename THasHolder<TList, TType>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename TType, typename = void>
class TIndexOfHolder {
public:
    static_assert(THas<TList, TType>::value, "List has not such type.");

    using TValue = std::integral_constant<int, 0>;
};

template <typename TList, typename TType>
class TIndexOfHolder<TList, TType, typename std::enable_if_t<!std::is_same_v<typename TList::THead, TType>>> {
public:
    static_assert(THas<TList, TType>::value, "List has not such type.");

    using TValue = std::integral_constant<int, 1 + TIndexOfHolder<typename TList::TTail, TType>::TValue::value>;
};

template <typename TList, typename TType>
using TIndexOf = typename TIndexOfHolder<TList, TType>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename TType>
class TPushFrontHolder;

template <typename TType, typename... TTypes>
class TPushFrontHolder<TTypeList<TTypes...>, TType> {
public:
    using TValue = TTypeList<TType, TTypes...>;
};

template <typename TList, typename TType>
using TPushFront = typename TPushFrontHolder<TList, TType>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
class TPopFrontHolder {
public:
    static_assert(TList::Length > 0, "The list is empty");
};

template <typename THead, typename... TTypes>
class TPopFrontHolder<TTypeList<THead, TTypes...>> {
public:
    using TValue = TTypeList<TTypes...>;
};

template <typename TList>
using TPopFront = typename TPopFrontHolder<TList>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename TType>
class TPushBackHolder;

template <typename TType, typename... TTypes>
class TPushBackHolder<TTypeList<TTypes...>, TType> {
public:
    using TValue = TTypeList<TTypes..., TType>;
};

template <typename TList, typename TType>
using TPushBack = typename TPushBackHolder<TList, TType>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename>
class TReverseHolder;

template <>
class TReverseHolder<TTypeList<>> {
public:
    using TValue = TTypeList<>;
};

template <typename TType, typename... TTypes>
class TReverseHolder<TTypeList<TType, TTypes...>> {
public:
    using TValue = TPushBack<
        typename TReverseHolder<TTypeList<TTypes...>>::TValue,
        TType>;
};

template <typename TList>
using TReverse = typename TReverseHolder<TList>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
using TPopBack = TReverse<TPopFront<TReverse<TList>>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
using THead = typename TList::THead;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
using TTail = THead<TReverse<TList>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename, typename = void>
class TRemoveHolder {
public:
    using TValue = TPopFront<TList>;
};

template <typename TList, typename TType>
class TRemoveHolder<TList, TType, typename std::enable_if_t<!std::is_same_v<typename TList::THead, TType>>> {
public:
    static_assert(THas<TList, TType>::value, "The list has not such type.");

    using TValue = TPushFront<
        typename TRemoveHolder<TPopFront<TList>, TType>::TValue,
        typename TList::THead>;
};

template <typename TList, typename TType>
using TRemove = typename TRemoveHolder<TList, TType>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename, typename...>
class TMergeHolder;

template <typename TList>
class TMergeHolder<TList> {
public:
    using TValue = TList;
};

template <typename TList>
class TMergeHolder<TList, TTypeList<>> {
public:
    using TValue = TList;
};

template <typename TList, typename TType, typename... TTypes>
class TMergeHolder<TList, TTypeList<TType, TTypes...>> {
public:
    using TValue = typename TMergeHolder<TPushBack<TList, TType>, TTypeList<TTypes...>>::TValue;
};

template <typename TList, typename... TLists>
using TMerge = typename TMergeHolder<TList, typename TMergeHolder<TLists...>::TValue>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int length>
class TPrefixHolder {
public:
    static_assert(length >= 0 && length <= TList::Length, "Index is out of range.");

    using TValue = TPushFront<
        typename TPrefixHolder<typename TList::TTail, length - 1>::TValue,
        typename TList::THead>;
};

template <typename TList>
class TPrefixHolder<TList, 0> {
public:
    using TValue = TTypeList<>;
};

template <typename TList, int length>
using TPrefix = typename TPrefixHolder<TList, length>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int length>
using TSuffix = TReverse<TPrefix<TReverse<TList>, length>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int length>
using TChopPrefix = TSuffix<TList, TList::Length - length>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int length>
using TChopSuffix = TPrefix<TList, TList::Length - length>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int first, int length>
using TSlice = TPrefix<TChopPrefix<TList, first>, length>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename TType, int index>
using TInsertBefore = TMerge<
    TPrefix<TList, index>,
    TPushFront<TSuffix<TList, TList::Length - index>, TType>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename TType, int index>
using TInsertAfter = TMerge<
    TPrefix<TList, index + 1>,
    TPushFront<TSuffix<TList, TList::Length - index - 1>, TType>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, typename TType, int index>
using TReplace = TMerge<
    TPrefix<TList, index>,
    TPushFront<TSuffix<TList, TList::Length - index - 1>, TType>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int first, int second>
using TSwap = TReplace<TReplace<TList, TAt<TList, second>, first>, TAt<TList, first>, second>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
using TClear = TTypeList<>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
using TIsEmpty = std::bool_constant<TList::Length == 0>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int... indices>
using TSubList = TTypeList<TAt<TList, indices>...>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int iters>
class TRotateHeadHolder {
public:
    using TValue = typename TRotateHeadHolder<
        TPushBack<typename TList::TTail, typename TList::THead>,
        iters - 1>::TValue;
};

template <typename TList>
class TRotateHeadHolder<TList, 0> {
public:
    using TValue = TList;
};

template <typename TList, int iters>
using TRotateHead = typename TRotateHeadHolder<TList, iters>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int iters>
using TRotateTail = TReverse<TRotateHead<TReverse<TList>, iters>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList1, typename TList2, int index>
using TSpliceAfter = TMerge<TPrefix<TList1, index>, TList2, TSuffix<TList1, TList1::Length - index>>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList1, typename TList2, int index>
using TSpliceBefore = TMerge<TPrefix<TList1, index - 1>, TList2, TSuffix<TList1, TList1::Length - index + 1>>;

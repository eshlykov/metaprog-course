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

template <typename, int>
class TAtHolder;

template <typename TList>
class TAtHolder<TList, 0> {
public:
    static_assert(TList::Length > 0, "Index out of range.");

    using TValue = typename TList::THead;
};

template <typename TList, int index>
class TAtHolder {
public:
    static_assert(index >= 0 && index < TList::Length, "Index out of range.");

    using TValue = std::conditional_t<
        index == 0,
        typename TList::THead,
        typename TAtHolder<typename TList::TTail, index - 1>::TValue>;
};

template <typename TList, int index>
using TAt = typename TAtHolder<TList, index>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename, typename>
class THasHolder;

template <typename TType>
class THasHolder<TTypeList<>, TType> {
public:
    using TValue = std::false_type;
};

template <typename TList, typename TType>
class THasHolder {
public:
    using TValue = std::disjunction<
        std::is_same<typename TList::THead, TType>,
        typename THasHolder<typename TList::TTail, TType>::TValue>;
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
class TReversedHolder;

template <>
class TReversedHolder<TTypeList<>> {
public:
    using TValue = TTypeList<>;
};

template <typename TType, typename... TTypes>
class TReversedHolder<TTypeList<TType, TTypes...>> {
public:
    using TValue = TPushBack<typename TReversedHolder<TTypeList<TTypes...>>::TValue, TType>;
};

template <typename TList>
using TReversed = typename TReversedHolder<TList>::TValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList>
using TPopBack = TReversed<TPopFront<TReversed<TList>>>;

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

    using TValue = TPushFront<typename TRemoveHolder<TPopFront<TList>, TType>::TValue, typename TList::THead>;
};

template <typename TList, typename TType>
using TRemove = typename TRemoveHolder<TList, TType>::TValue;

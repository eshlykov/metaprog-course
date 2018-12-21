#pragma once

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <auto...>
class TValueList;

template <>
class TValueList<> {
public:
    static constexpr int Length = 0;
};

template <auto value, auto... values>
class TValueList<value, values...> {
public:
    static constexpr auto Head = value;
    using TTail = TValueList<values...>;

    static constexpr int Length = 1 + TTail::Length;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList, int index>
constexpr auto At() {
    static_assert(index < TList::Length, "Index out of range.");
    if constexpr (index > 0) {
        return At<typename TList::TTail, index - 1>();
    } else {
        return TList::Head;
    }
}

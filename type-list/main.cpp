#include "type-list.hpp"
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>

class T0 {
};

class T1 {
};

class T2 {
};

class T3 {
};

class T4 {
};

class T5 {
};

class T6 {
};

class T7 {
};

template <typename TStream, typename... TTypes>
void PrintTypeList(TStream& stream, TTypeList<TTypes...>) {
    (std::cout << ... << (std::string(typeid(TTypes).name()) + " ")) << "\n";
}

int main() {
    using TList0 = TTypeList<>;
    using TList1 = TTypeList<T0>;
    using TList2 = TTypeList<T0, T1>;
    using TList3 = TTypeList<T0, T1, T2>;
    using TList4 = TTypeList<T0, T1, T2, T3>;
    using TList5 = TTypeList<T0, T1, T2, T3, T4>;
    using TList6 = TTypeList<T0, T1, T2, T3, T4, T5>;
    using TList7 = TTypeList<T0, T1, T2, T3, T4, T5, T6>;
    using TList8 = TTypeList<T0, T1, T2, T3, T4, T5, T6, T7>;

    static_assert(TList0::Length == 0, "");
    static_assert(TList1::Length == 1, "");
    static_assert(TList2::Length == 2, "");
    static_assert(TTypeList<TTypeList<>>::Length == 1, "");

    static_assert(std::is_same_v<TAt<TList3, 0>, T0>, "");
    static_assert(std::is_same_v<TAt<TList3, 1>, T1>, "");
    static_assert(std::is_same_v<TAt<TList3, 2>, T2>, "");
    static_assert(!std::is_same_v<TAt<TList3, 2>, T1>, "");

    static_assert(THas<TList3, T0>::value, "");
    static_assert(THas<TList3, T1>::value, "");
    static_assert(THas<TList3, T2>::value, "");
    static_assert(!THas<TList3, T3>::value, "");

    static_assert(TIndexOf<TList3, T0>::value == 0, "");
    static_assert(TIndexOf<TList3, T1>::value == 1, "");
    static_assert(TIndexOf<TList3, T2>::value == 2, "");

    static_assert(TIndexOf<TPushFront<TList0, T3>, T3>::value == 0, "");
    static_assert(TIndexOf<TPushFront<TList1, T3>, T3>::value == 0, "");
    static_assert(TIndexOf<TPushFront<TList2, T3>, T3>::value == 0, "");

    static_assert(!THas<TPopFront<TList1>, T0>::value, "");
    static_assert(!THas<TPopFront<TList2>, T0>::value, "");
    static_assert(!THas<TPopFront<TList3>, T0>::value, "");

    static_assert(TIndexOf<TPushBack<TList0, T3>, T3>::value == 0, "");
    static_assert(TIndexOf<TPushBack<TList1, T3>, T3>::value == 1, "");
    static_assert(TIndexOf<TPushBack<TList2, T3>, T3>::value == 2, "");

    static_assert(TIndexOf<TReversed<TList3>, T0>::value == 2, "");
    static_assert(TIndexOf<TReversed<TList3>, T1>::value == 1, "");
    static_assert(TIndexOf<TReversed<TList3>, T2>::value == 0, "");

    static_assert(!THas<TPopBack<TList1>, T0>::value, "");
    static_assert(!THas<TPopBack<TList2>, T1>::value, "");
    static_assert(!THas<TPopBack<TList3>, T2>::value, "");

    static_assert(TIndexOf<TRemove<TList3, T1>, T0>::value == 0, "");
    static_assert(!THas<TRemove<TList3, T1>, T1>::value, "");
    static_assert(TIndexOf<TRemove<TList3, T1>, T2>::value == 1, "");

    PrintTypeList(std::cout, TRemove<TList7, T3>{});

    return 0;
}

#include "tree-hierarchy.hpp"
#include "../type-list/type-list.hpp"
#include <iostream>
#include <string>
#include <typeinfo>

using namespace std::string_literals;

template <typename... TTypes>
std::string CastTypeListToString(TTypeList<TTypes...>) {
    return ("[ "s + ... + (std::string{typeid(TTypes).name()} + ", "s)) + "]"s;
}

template <typename... TTypeLists>
std::string CastTypeListOfTypeListsToString(TTypeList<TTypeLists...>) {
    return ("[\n  "s + ... + (CastTypeListToString(TTypeLists{}) + ",\n  "s)) + "\n]"s;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <int index>
class TFibonacciNumber {
    static_assert(index >= 0, "Fibonacci number index is out of range");
public:
    static constexpr int value = TFibonacciNumber<index - 1>::value + TFibonacciNumber<index - 2>::value;
};

template <>
class TFibonacciNumber<0> {
public:
    static constexpr int value = 1;
};

template <>
class TFibonacciNumber<1> {
public:
    static constexpr int value = 1;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename, typename TAccumulator>
class TFibonacciGroupConditionHolder {
public:
    using TValue = std::integral_constant<
        bool,
        TLength<TTail<TAccumulator>>::value != TFibonacciNumber<TLength<TAccumulator>::value - 1>::value>;
};

template <typename TElement>
class TFibonacciGroupConditionHolder<TElement, TTypeList<>> {
public:
    using TValue = std::true_type;
};

template <typename TElement, typename TAccumulator>
using TFibonacciGroupCondition = typename TFibonacciGroupConditionHolder<TElement, TAccumulator>::TValue;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <
    typename TList,
    template <typename> typename TBranchHead,
    template <typename, typename> typename TBranchNode,
    typename TBranchTail>
using TFibonacciTreeHierarchy = TTreeHierarchy<TList, TBranchHead, TBranchNode, TBranchTail, TFibonacciGroupCondition>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class IHeadWelcomer : public T {
public:
    std::string GetWelcome() {
        return "Welcome from dummy head" + T::GetWelcome();
    }
};

template <typename TType, typename TBase = TTypeList<>>
class TNodeWelcomer : public TBase {
public:
    static std::string GetWelcome() {
        return ", welcome from Node<" + std::string{typeid(TType).name()} + ">" + TBase::GetWelcome();
    };
};

class TTailWelcomer {
public:
    static std::string GetWelcome() {
        return " and welcome from dummy tail";
    }
};

int main() {
    using TList = TTypeList<
        bool, char, int, short, long,
        bool, char, int, short, long,
        bool, char, int, short, long,
        bool, char, int, short, long,
        bool, char, int, short, long,
        bool, char, int, short, long,
        bool, char, int, short, long,
        bool, char, int, short, long,
        bool, char, int, short, long>;

    using TGroupedList = TGroupBy<TList, TFibonacciGroupCondition>;

    std::cout << CastTypeListToString(TList{}) << std::endl;
    std::cout << CastTypeListOfTypeListsToString(TGroupedList{}) << std::endl;

    using TFibonacciWelcomer = TFibonacciTreeHierarchy<TList, IHeadWelcomer, TNodeWelcomer, TTailWelcomer>;

    TFibonacciWelcomer* welcomer = new TFibonacciWelcomer{};

    TAt<typename TFibonacciWelcomer::TBases, 0>* welcomer0 = welcomer;
    TAt<typename TFibonacciWelcomer::TBases, 1>* welcomer1 = welcomer;
    TAt<typename TFibonacciWelcomer::TBases, 2>* welcomer2 = welcomer;
    TAt<typename TFibonacciWelcomer::TBases, 3>* welcomer3 = welcomer;
    TAt<typename TFibonacciWelcomer::TBases, 4>* welcomer4 = welcomer;
    TAt<typename TFibonacciWelcomer::TBases, 5>* welcomer5 = welcomer;
    TAt<typename TFibonacciWelcomer::TBases, 6>* welcomer6 = welcomer;
    TAt<typename TFibonacciWelcomer::TBases, 7>* welcomer7 = welcomer;

    std::cout << welcomer0->GetWelcome() << std::endl;
    std::cout << welcomer1->GetWelcome() << std::endl;
    std::cout << welcomer2->GetWelcome() << std::endl;
    std::cout << welcomer3->GetWelcome() << std::endl;
    std::cout << welcomer4->GetWelcome() << std::endl;
    std::cout << welcomer5->GetWelcome() << std::endl;
    std::cout << welcomer6->GetWelcome() << std::endl;
    std::cout << welcomer7->GetWelcome() << std::endl;
}

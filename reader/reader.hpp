#pragma once

#include "../type-list/type-list.hpp"
#include "value-list.hpp"
#include <cstdlib>
#include <fstream>
#include <type_traits>
#include <vector>

class TNone {
};

class TNoDecompressor {
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <template <typename...> typename TTemplate, typename TClass>
struct IsTemplateOf : std::false_type {
};

template <template <typename...> typename TClass, typename... TParameters>
struct IsTemplateOf<TClass, TClass<TParameters...>> : std::true_type {
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TList1, typename TList2 = TNone, typename TList3 = TNone>
class TReader {
    static_assert(
        std::is_same_v<TList2, TNone> == std::is_same_v<TList3, TNone>,
        "Second and last parameters should both be null type or both be not null type."
    );

public:
    explicit TReader(const std::string& filename) :
        file{std::ifstream{filename}},
        lineSize{GetLineSize()},
        list3index{0} {
    }

    ~TReader() {
        file.close();
        for (void* ptr : lines) {
            std::free(ptr);
        }
    }

    void* ReadNextLine() {
        void* memory = std::malloc(lineSize);
        StoreValues(memory);
        lines.emplace_back(memory);
        return memory;
    }

private:
    template <int index = 0>
    int GetLineSize() const {
        if constexpr (index < TList1::Length) {
            return sizeof(TAt<TList1, index>) + GetLineSize<index + 1>();
        }
        return 0;
    }

    void StoreValues(void* memory) {
        DoStoreValues(memory, memory);
    }

    template <int index = 0>
    void DoStoreValues(void* memory, void* current) {
        if constexpr (index < TList1::Length) {
            auto value = ReadValue<index>();
            *reinterpret_cast<decltype(value)*>(current) = value;
            current = reinterpret_cast<void*>(reinterpret_cast<char*>(current) + sizeof(value));
            DoStoreValues<index + 1>(memory, current);
        }
    }

    template <int index>
    auto ReadValue() {
        using TType = TAt<TList1, index>;
        if constexpr (std::is_same_v<TList2, TNone>) {
            TType value = {};
            file >> value;
            return value;
        } else {
            using TCompressedType = TAt<TList2, index>;
            if constexpr (!std::is_same_v<TCompressedType, TNone>) {
                typename TCompressedType::TType value;
                file >> value;
                return TryMoreDecompress<index>(TCompressedType{value}.Decompress());
            } else if constexpr (IsTemplateOf<TTypeList, TList3>::value) {
                typename TAt<TList3, index>::TType value;
                file >> value;
                return TAt<TList3, index>{value}.Decompress();
            } else {
                TType value;
                file >> value;
                return At<TList3, index>()(value);
            }
        }
    }

    template <int index, typename TSourceType>
    auto TryMoreDecompress(TSourceType value) {
        if constexpr (IsTemplateOf<TTypeList, TList3>::value) {
          return value;
        } else {
            constexpr auto decompress = At<TList3, index>();
            if constexpr (decompress != nullptr) {
                return decompress(value);
            } else {
                return value;
            }
        }
    }

private:
    std::ifstream file;
    int lineSize;
    int list3index;
    std::vector<void*> lines;
};

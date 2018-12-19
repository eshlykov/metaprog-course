#include "../type-list/type-list.hpp"
#include "decompressor.hpp"
#include "reader.hpp"
#include "value-list.hpp"
#include <iostream>

using TBaseReader = TReader<TTypeList<int, double, int>>;

using TImprovedReader = TReader<
    TTypeList<int, double, int>,
    TTypeList<TDecompressor<std::string, int>, TNone, TNone>,
    TTypeList<TNoDecompressor, TDecompressor<double, double>, TDecompressor<char, int>>
>;

using TBestReader = TReader<
    TTypeList<int, double, int>,
    TTypeList<TDecompressor<std::string, int>, TNone, TDecompressor<char, int>>,
    TValueList<nullptr, Decompress<double, double>, Decompress<int, int>>
>;

template <typename TReader>
void TestReader() {
    TReader reader{"input.txt"};
    for (int i = 0; i < 2; ++i) {
        void* line = reader.ReadNextLine();
        int* first = reinterpret_cast<int*>(line);
        double* second = reinterpret_cast<double*>(first + 1);
        int* third = reinterpret_cast<int*>(second + 1);
        std::cout << *first << " " << *second << " " << *third << std::endl;
    }
}

int main() {
    TestReader<TBaseReader>();
    TestReader<TImprovedReader>();
    TestReader<TBestReader>();
}

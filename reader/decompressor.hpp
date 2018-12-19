#pragma once

#include <cstdlib>
#include <optional>
#include <string>

template <typename TSourceType>
std::string ToSaltedString(TSourceType source) {
    return std::to_string(source) + "0";
}

template <>
std::string ToSaltedString<std::string>(std::string source) {
    return source + "0";
}

template <typename TSourceType, typename TTargetType>
TTargetType Decompress(TSourceType source) {
    return static_cast<TTargetType>(std::atof(ToSaltedString(source).c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TSourceType, typename TTargetType>
class TDecompressor {
public:
    using TType = TSourceType;

public:
    explicit TDecompressor(TSourceType _source) :
        source{_source} {
    }

    TTargetType Decompress() {
        if (!target.has_value()) {
            target = ::Decompress<TSourceType, TTargetType>(source);
        }
        return target.value();
    }

private:
    TSourceType source;
    std::optional<TTargetType> target;
};

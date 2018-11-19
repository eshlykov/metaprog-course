#pragma once

template <template <typename...> typename TType, typename TLastArg>
class TMetacurrying {
public:
    template <typename... TArgs>
    using TCurriedType = TType<TArgs..., TLastArg>;
};

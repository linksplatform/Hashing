#pragma once

// based on https://stackoverflow.com/a/7115547/710069

#ifndef PLATFORM_HASHING_TUPLE
#define PLATFORM_HASHING_TUPLE

#include <tuple>

namespace Platform::Hashing
{
    template <class TupleType, size_t Index = std::tuple_size<TupleType>::value - 1>
    struct Tuple
    {
        static void Combine(std::uint32_t &hashAccumulator, const TupleType &tuple)
        {
            Tuple<TupleType, Index - 1>::Combine(hashAccumulator, tuple);
            Hashing::Combine(hashAccumulator, std::get<Index>(tuple));
        }
    };

    template <class TupleType>
    struct Tuple<TupleType, 0>
    {
        static void Combine(std::uint32_t &hashAccumulator, const TupleType &tuple)
        {
            Hashing::Combine(hashAccumulator, std::get<0>(tuple));
        }
    };
}

namespace std
{
    template <typename ... TT>
    struct hash<std::tuple<TT...>>
    {
        size_t operator()(const std::tuple<TT...> &tuple) const
        {
            std::uint32_t seed = 0;
            Platform::Hashing::Tuple<std::tuple<TT...>>::Combine(seed, tuple);
            return seed;
        }
    };
}

#endif

#pragma once

// Based on https://stackoverflow.com/a/7115547/710069

#include <tuple>

#include "Platform.Hashing.Hash.h"

namespace Platform::Hashing
{
    template <class TupleType, size_t Index = std::tuple_size<TupleType>::value - 1>
    struct Tuple
    {
        static void Combine(std::size_t & hashAccumulator, const TupleType& tuple)
        {
            Tuple<TupleType, Index - 1>::Combine(hashAccumulator, tuple);
            std::size_t hash = Hashing::Hash(std::get<Index>(tuple));
            Hashing::Combine(hashAccumulator, hash);
        }
    };

    template <class TupleType>
    struct Tuple<TupleType, 0>
    {
        static void Combine(std::size_t& hashAccumulator, const TupleType& tuple)
        {
            std::size_t hash = Hashing::Hash(std::get<0>(tuple));
            Hashing::Combine(hashAccumulator, hash);
        }
    };
}

namespace std
{
    template <typename ... Types>
    struct hash<std::tuple<Types...>>
    {
        std::size_t operator()(const std::tuple<Types...> &tuple) const
        {
            std::size_t hash = 0;
            Platform::Hashing::Tuple<std::tuple<Types...>>::Combine(hash, tuple);
            return hash;
        }
    };
}

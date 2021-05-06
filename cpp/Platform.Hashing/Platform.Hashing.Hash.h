#pragma once

#ifndef PLATFORM_HASHING_HASH
#define PLATFORM_HASHING_HASH

#include <functional>
#include <cstring>

#include "Platform.Hashing.Expand.h"

namespace Platform::Hashing
{
    template <typename T>
    concept std_hashable = requires(T object) { std::hash<T>{}(object); };

    template <typename T>
    concept not_std_hashable = !std_hashable<T>;

    template<typename T> std::size_t HashRaw(const T &value, std::size_t size = 1)
    {
        std::uint32_t hash = typeid(T).hash_code();
        Combine(hash, &value, size);
        return Expand(hash);
    }

    template<typename T> std::size_t Hash(const T &value)
    {
        if constexpr (std_hashable<std::decay_t<T>>)
        {
            if constexpr (std::is_array_v<T>)
            {
                using TItem = std::remove_reference_t<decltype(std::declval<T>()[0])>;
                std::hash<const TItem*> hasher;
                return hasher(value);
            }
            else
            {
                std::hash<std::decay_t<T>> hasher;
                return hasher(value);
            }
        }
        else
        {
            return HashRaw(value);
        }
    }

    template<typename ... Args> std::size_t Hash(const Args&... args)
    {
        std::hash<std::tuple<Args...>> hasher;
        return hasher(std::tie(args...));
    }
}

#endif

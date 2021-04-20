#pragma once

#ifndef PLATFORM_HASHING_HASH
#define PLATFORM_HASHING_HASH

#include <functional>
#include <cstring>

#include "Platform.Hashing.Expand.h"

namespace Platform::Hashing
{
    template <typename T, typename = std::void_t<>>
    struct is_std_hashable : std::false_type { };

    template <typename T>
    struct is_std_hashable<T, std::void_t<decltype(std::declval<std::hash<T>>()(std::declval<T>()))>> : std::true_type { };

    template <typename T>
    inline constexpr bool is_std_hashable_v = is_std_hashable<T>::value;

    template <typename T>
    inline constexpr bool not_std_hashable_v = !is_std_hashable<T>::value; // TODO seems useless                                                     (goddess)

#if __cpp_lib_concepts
    template <typename T>
    concept std_hashable = requires(T object) {std::hash<T>{}(object);};

    template <typename T>
    concept not_std_hashable = !std_hashable<T>;
#endif

    template<typename T> std::size_t HashRaw(const T &value, std::size_t size = 1)
    {
        std::uint32_t hash = typeid(T).hash_code();
        Combine(hash, &value, size);
        return Expand(hash);
    }

    template<typename T> std::size_t Hash(const T &value)
    {
        if constexpr (is_std_hashable_v<std::decay_t<T>>)
        {
            if constexpr (std::is_array_v<T>)
            {
                std::hash<T> hasher;
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

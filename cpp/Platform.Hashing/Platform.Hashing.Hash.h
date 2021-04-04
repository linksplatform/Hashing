#pragma once

#ifndef PLATFORM_HASHING_HASH
#define PLATFORM_HASHING_HASH

#include <functional>

namespace Platform::Hashing
{

#ifndef __cpp_lib_concepts
    template <typename T, typename = std::void_t<>>
    struct is_std_hashable_struct : std::false_type { };

    template <typename T>
    struct is_std_hashable_struct<T, std::void_t<decltype(std::declval<std::hash<T>>()(std::declval<T>()))>> : std::true_type { };

    template <typename T>
    inline constexpr bool is_std_hashable = is_std_hashable_struct<T>::value;

    template <typename T>
    inline constexpr bool not_std_hashable = !is_std_hashable_struct<T>::value;
#else
    template <typename T>
    concept is_std_hashable = requires(T object) {std::hash<T>{}(object);};

    template <typename T>
    concept not_std_hashable = !is_std_hashable<T>;
#endif


    template<typename T> std::size_t RawHash(T &&value)
    {
        std::uint32_t hash = 0;
        Combine(hash, value);
        return Expand(hash);
    }

    template<typename T> std::size_t Hash(T &&value)
    {
        if constexpr (is_std_hashable<typename std::decay<T>::type>)
        {
            std::hash<typename std::decay<T>::type> hasher;
            return hasher(std::forward<T>(value));
        }
        else
        {
            return RawHash(value);
        }
    }

    template<typename ... Args> std::size_t Hash(Args&&... args)
    {
        std::hash<std::tuple<Args...>> hasher;
        return hasher(std::tie(args...));
    }
}

#endif

#pragma once

#ifndef PLATFORM_HASHING_OTHER_H
#define PLATFORM_HASHING_OTHER_H

#include "Platform.Hashing.Hash.h"

#include <any>

namespace std
{
    template<>
    struct hash<any>
    {
        size_t operator()(const any &object) const noexcept
        {
            throw std::logic_error("the hash from std::any is too insecure");
        }
    };

#ifdef __cpp_lib_concepts
    // FIXME use the concept from Platform::System
    template<typename _Type>
    concept IEnumerable = requires(_Type object)
    {
        {object.begin()} -> std::forward_iterator;
        {object.end()} -> std::forward_iterator;
    };

    template<Platform::Hashing::not_std_hashable T> requires IEnumerable<T>
    struct hash<T>
    {
        size_t operator()(const T &object) const
        {
            using ItemType = decltype(*object.begin());

            std::size_t hash = typeid(T).hash_code();
            for(auto element : object)
            {
                // https://stackoverflow.com/a/2595226
                hash ^= Platform::Hashing::Hash(element) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return Platform::Hashing::Expand(hash);
        }
    };

    template<Platform::Hashing::not_std_hashable T>
    struct hash<T>
    {
        size_t operator()(const T &object) const noexcept
        {
            return Platform::Hashing::HashRaw(object);
        }
    };

#else
    /// ...
#endif

    template<typename T>
    struct hash<T&>
    {
        size_t operator()(const T &object) const noexcept
        {
            return std::hash<T*>{}((T*)&object);
        }
    };
}

#endif //PLATFORM_HASHING_PLATFORM_HASHING_OTHER_H

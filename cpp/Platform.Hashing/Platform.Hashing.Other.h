#pragma once

#ifndef PLATFORM_HASHING_OTHER_H
#define PLATFORM_HASHING_OTHER_H

#include "Platform.Hashing.Hash.h"

namespace std
{
#ifdef __cpp_lib_concepts
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

}

#endif //PLATFORM_HASHING_PLATFORM_HASHING_OTHER_H

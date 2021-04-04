#pragma once

#ifndef PLATFORM_HASHING_PLATFORM_HASHING_OTHER_H
#define PLATFORM_HASHING_PLATFORM_HASHING_OTHER_H

namespace std
{
#ifdef __cpp_lib_concepts
    template<Platform::Hashing::not_std_hashable T>
    struct hash<T>
    {
        size_t operator()(const T& item) const noexcept
        {
            return Platform::Hashing::TrivialHash(item);
        }
    };
#else

#endif


}


#endif //PLATFORM_HASHING_PLATFORM_HASHING_OTHER_H

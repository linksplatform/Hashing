#pragma once

#ifndef PLATFORM_HASHING_HASH
#define PLATFORM_HASHING_HASH

namespace Platform::Hashing
{
    template<typename T> std::uint32_t Hash(T &&value)
    {
        std::uint32_t hashAccumulator = 0;
        Combine(hashAccumulator, value);
        return hashAccumulator;
    }

    template<typename ... Args> std::uint32_t Hash(Args... args)
    {
        std::tuple<Args...> argsTuple = { args... };
        std::hash<std::tuple<Args...>> hasher;
        return static_cast<std::uint32_t>(hasher(argsTuple));
    }
}

#endif

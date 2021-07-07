#pragma once

#include "Platform.Hashing.Hash.h"

#include <any>
#include <typeindex>
#include <stdexcept>

namespace Platform::Hashing
{
    namespace Internal
    {
        template<typename T>
        auto ToAnyHasher(auto&& func)
        {
            return
                std::pair<std::type_index, std::function<std::size_t(const std::any&)>>
                {
                    std::type_index(typeid(T)),
                    [&func](const std::any& a) -> std::size_t
                    {
                        return func(std::any_cast<T>(a));
                    }
                };
        }

        #define HASHER(Type) ToAnyHasher<Type>([](auto&& arg) { return Hash(arg); })
        std::unordered_map<std::type_index, std::function<std::size_t(const std::any&)>>
            AnyHashers
            {
                HASHER(short int),
                HASHER(unsigned short int),
                HASHER(int),
                HASHER(unsigned int),
                HASHER(unsigned long int),
                HASHER(long long int),
                HASHER(unsigned long long int),
                HASHER(float),
                HASHER(double),
                HASHER(long double),
                HASHER(const char*),
                HASHER(std::string),
            };
        #undef HASHER

    }
    template<class T>
    inline void RegisterAnyHasher(auto&& func)
    {
        Internal::AnyHashers.insert(Internal::ToAnyHasher<T>(std::forward<decltype(func)>(func)));
    }
}

template<>
struct std::hash<std::any>
{
    size_t operator()(const std::any& object) const
    {
        if (!Platform::Hashing::Internal::AnyHashers.contains(object.type()))
        {
            // TODO later replace to std::format
            throw std::runtime_error(std::string("Hash function for type ").append(object.type().name()).append(" is unregistered"));
        }

        auto hasher = Platform::Hashing::Internal::AnyHashers[object.type()];
        return hasher(object);
    }
};

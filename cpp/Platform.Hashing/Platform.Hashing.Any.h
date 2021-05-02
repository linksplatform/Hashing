#pragma once

#ifndef PLATFORM_HASHING_ANY
#define PLATFORM_HASHING_ANY

#include "Platform.Hashing.Hash.h"

#include <any>
#include <typeindex>
#include <stdexcept>

namespace Platform::Hashing
{
    namespace Internal
    {
        template<class T>
        inline auto ToAnyHasher(auto&& func)
        {
            return std::pair<const std::type_index, std::function<std::size_t(std::any)>>
                {
                    std::type_index(typeid(T)),
                    [func](std::any a) -> std::size_t
                    {
                        if constexpr (std::is_void_v<T>)
                            return func();
                        else
                            return func(std::any_cast<T>(a));
                    }
                };
        }

        #define HASHER(Type) ToAnyHasher<Type>(Hash<Type>)
        static std::unordered_map<std::type_index, std::function<std::size_t(std::any)>>
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
                HASHER(const std::string&),
            };
        #undef HASHER

    }
    template<class T>
    inline void RegisterAnyHasher(auto&& func)
    {
        Internal::AnyHashers.insert(Internal::ToAnyHasher<T>(std::forward<decltype(func)>(func)));
    }
}

namespace std
{
    template<>
    struct hash<any>
    {
        size_t operator()(const any& object) const
        {
            if (!Platform::Hashing::Internal::AnyHashers.contains(object.type()))
            {
                // TODO later replace to std::forward
                throw std::runtime_error(std::string("Hash function for type ")
                                             .append(object.type().name())
                                             .append(" is unregistered"));
            }

            auto hasher = Platform::Hashing::Internal::AnyHashers[object.type()];
            return hasher(object);
        }
    };
}

#endif //PLATFORM_HASHING_PLATFORM_HASHING_ANY

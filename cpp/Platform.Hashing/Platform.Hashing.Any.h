#pragma once

#ifndef PLATFORM_HASHING_OTHER_H
#define PLATFORM_HASHING_OTHER_H

#include "Platform.Hashing.Hash.h"

#include <any>
#include <typeindex>
#include <stdexcept>

namespace Platform::Hashing
{
    namespace Internal
    {
        template<class T>
        inline auto ToAnyHashVisitor(auto&& func)
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

        #define HASH_VISITOR(Type) ToAnyHashVisitor<Type>(Hash<Type>)
        static std::unordered_map<std::type_index, std::function<std::size_t(std::any)>>
            AnyHashVisitors
            {
                HASH_VISITOR(short int),
                HASH_VISITOR(unsigned short int),
                HASH_VISITOR(int),
                HASH_VISITOR(unsigned int),
                HASH_VISITOR(unsigned long int),
                HASH_VISITOR(long long int),
                HASH_VISITOR(unsigned long long int),
                HASH_VISITOR(float),
                HASH_VISITOR(double),
                HASH_VISITOR(long double),
                HASH_VISITOR(const char*),
                HASH_VISITOR(const std::string&),
            };
        #undef HASH_VISITOR

    }
    template<class T>
    inline void RegisterAnyHashVisitor(auto&& func)
    {
        Internal::AnyHashVisitors.insert(Internal::ToAnyHashVisitor<T>(std::forward<decltype(func)>(func)));
    }
}

namespace std
{
    template<>
    struct hash<any>
    {
        size_t operator()(const any& object) const
        {
            if (!Platform::Hashing::Internal::AnyHashVisitors.contains(object.type()))
                // TODO later replace to std::forward
                throw std::runtime_error(std::string("Hash function for type ")
                                                .append(object.type().name())
                                                .append(" is unregistered"));

            auto hasher = Platform::Hashing::Internal::AnyHashVisitors[object.type()];
            return hasher(object);
        }
    };
}

#endif //PLATFORM_HASHING_PLATFORM_HASHING_OTHER_H

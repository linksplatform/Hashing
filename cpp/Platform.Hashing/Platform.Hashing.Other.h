#pragma once

#ifndef PLATFORM_HASHING_OTHER_H
#define PLATFORM_HASHING_OTHER_H

#include "Platform.Hashing.Hash.h"

#include <any>
#include <typeindex>

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

        #define HASH_VISITOR(Type) ToAnyHashVisitor<Type>([](Type a) {return Hash(a);})
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

    // TODO appreciate the idiom

    template<Platform::Hashing::not_std_hashable T>
    requires
        requires(T collection)
        {
            std::ranges::data(collection);
            std::ranges::size(collection);
        }
    struct hash<T>
    {
        size_t operator()(const T& collection) const
        {
            using TItem = std::ranges::range_value_t<T>;
            std::uint32_t hash = 0;

            if constexpr (is_fundamental_v<TItem>)
            {
                Platform::Hashing::Combine(hash, std::ranges::data(collection), std::ranges::size(collection));
                return Platform::Hashing::Expand(hash);
            }
            else
            {
                auto data = std::ranges::data(collection);
                const auto size = std::ranges::size(collection);
                for (int i = 0; i < size; i++)
                {
                    hash = Platform::Hashing::CombineHash(hash, Platform::Hashing::Hash(*data));
                    ++data;
                }
                return Platform::Hashing::Expand(hash);
            }
        }
    };

    template<Platform::Hashing::not_std_hashable T>
    requires
        (!requires(T collection)
        {
            std::ranges::data(collection);
            std::ranges::size(collection);
        })
        &&
        std::ranges::range<T>
    struct hash<T>
    {
        size_t operator()(const T& collection) const
        {
            using TItem = std::ranges::range_value_t<T>;
            std::uint32_t hash = 0;
            std::size_t size = 0;
            std::vector<TItem> data;

            if constexpr (requires() { std::ranges::size(collection); })
            {
                data.reserve(std::ranges::size(collection));
            }

            for (const auto& it : collection)
            {
                data.push_back(it);
            }

            return Platform::Hashing::Hash(data);
        }
    };
}

#endif //PLATFORM_HASHING_PLATFORM_HASHING_OTHER_H

#pragma once

#ifndef PLATFORM_HASHING_OTHER_H
#define PLATFORM_HASHING_OTHER_H

#include "Platform.Hashing.Hash.h"

#include <any>
#include <typeindex>


// FIXME use the concept from Platform::System
namespace Platform::Hashing
{
    template<typename _Type>
    concept IEnumerable = requires(_Type object)
    {
        {object.begin()} -> std::forward_iterator;
        {object.end()} -> std::forward_iterator;
    };
}

namespace Platform::Hashing
{
    template<class T>
    inline auto __to_any_hash_visitor(auto func)
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

    #define BASE_VISITOR_REGISTER(Type) __to_any_hash_visitor<Type>([](Type a) {return Hash(a);})
    static std::unordered_map<std::type_index, std::function<std::size_t(std::any)>>
            any_hash_visitor {
            BASE_VISITOR_REGISTER(short int),
            BASE_VISITOR_REGISTER(unsigned short int),
            BASE_VISITOR_REGISTER(int),
            BASE_VISITOR_REGISTER(unsigned int),
            BASE_VISITOR_REGISTER(unsigned long int),
            BASE_VISITOR_REGISTER(long long int),
            BASE_VISITOR_REGISTER(unsigned long long int),
            BASE_VISITOR_REGISTER(float),
            BASE_VISITOR_REGISTER(double),
            BASE_VISITOR_REGISTER(long double),
    };
    #undef BASE_VISITOR_REGISTER

    template<class T>
    inline void register_any_hash_visitor(auto func)
    {
        any_hash_visitor.insert(__to_any_hash_visitor<T>(func));
    }
}

namespace std
{
    template<>
    struct hash<any>
    {
        size_t operator()(const any &object) const
        {
            if(!Platform::Hashing::any_hash_visitor.contains(object.type()))
                throw std::runtime_error(std::string("Hash function for type ").append(object.type().name()).append(" is unregistered"));

            auto hasher = Platform::Hashing::any_hash_visitor[object.type()];
            return hasher(object);
        }
    };

#ifdef __cpp_lib_concepts

    template <typename _Type>
    concept IHashableCollection =
        Platform::Hashing::IEnumerable<_Type> ||
        requires(_Type object) {object.data(); object.size();} ||
        (requires(_Type object) {object.size();} && Platform::Hashing::IEnumerable<_Type>);

    template<Platform::Hashing::not_std_hashable T> requires IHashableCollection<T>
    struct hash<T>
    {
        size_t operator()(const T &collection) const
        {
            std::size_t hash = 0;

            if constexpr (requires(T object){object.data(); object.size();})
            {
                using TItem = decltype(*collection.data());
                if constexpr (is_fundamental_v<TItem>)
                {
                    Platform::Hashing::Combine(hash, collection.data(), collection.size());
                    return Platform::Hashing::Expand(hash);
                }
                else
                {
                    auto data = collection.data();
                    for(int i = 0; i < collection.size(); i++)
                    {
                        hash = Platform::Hashing::CombineHash(hash, Platform::Hashing::Hash(data[i]));
                    }
                    return Platform::Hashing::Expand(hash);
                }
            }

            if constexpr (requires(T object) {object.size();} && Platform::Hashing::IEnumerable<T>)
            {
                using TItem = decltype(*collection.begin());

                if constexpr (is_fundamental_v<TItem>)
                {
                    auto* data = new TItem[collection.size()];
                    for(auto it : collection)
                    {
                        *data = it;
                        data++;
                    }
                    Platform::Hashing::Combine(hash, data - collection.size(), collection.size());
                    return Platform::Hashing::Expand(hash);
                }
            }

            if constexpr (Platform::Hashing::IEnumerable<T>)
            {
                using TItem = decltype(*collection.begin());
                if constexpr (is_fundamental_v<TItem>)
                {
                    auto* data = new TItem[collection.size()];
                    for(auto it : collection)
                    {
                        *data = it;
                        data++;
                    }
                    Platform::Hashing::Combine(hash, data - collection.size(), collection.size());
                    return Platform::Hashing::Expand(hash);
                }
            }

            for(auto element : collection)
            {
                hash = Platform::Hashing::CombineHash(hash, Platform::Hashing::Hash(element));
            }
            return Platform::Hashing::Expand(hash);
        }
    };
#endif
}

#endif //PLATFORM_HASHING_PLATFORM_HASHING_OTHER_H

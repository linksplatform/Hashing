#pragma once

#include "Platform.Hashing.Hash.h"

#include <any>
#include <cmath>
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

        static_assert(std::same_as<signed int, int>);

        #define HASHER(Type) ToAnyHasher<Type>([](auto&& arg) { return Hash(arg); })
        std::unordered_map<std::type_index, std::function<std::size_t(const std::any&)>>
            AnyHashers
            {
                HASHER(  signed short),
                HASHER(  signed short int),
                HASHER(unsigned short),
                HASHER(unsigned short int),
                HASHER(  signed int),
                HASHER(unsigned int),
                HASHER(  signed long int),
                HASHER(unsigned long int),
                HASHER(  signed long long int),
                HASHER(unsigned long long int),

                HASHER(float),
                HASHER(double),
                HASHER(long double),

                // Overwrite for std types
                HASHER(std::int8_t),
                HASHER(std::uint8_t),
                HASHER(std::int16_t),
                HASHER(std::uint16_t),
                HASHER(std::int32_t),
                HASHER(std::uint32_t),
                HASHER(std::int64_t),
                HASHER(std::uint64_t),
                HASHER(std::float_t),
                HASHER(std::double_t),

                // Overwrite for char's
                HASHER(char),
                HASHER(char8_t),
                HASHER(char16_t),
                HASHER(char32_t),
                HASHER(wchar_t),
                HASHER(const char*),
                HASHER(const char8_t*),
                HASHER(const char16_t*),
                HASHER(const char32_t*),
                HASHER(const wchar_t*),
                HASHER(std::string),
                HASHER(std::u8string),
                HASHER(std::u16string),
                HASHER(std::u32string),
                HASHER(std::wstring),
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

        //                                           ^
        // TODO: use unordered_map::at()'s exception |
        auto hasher = Platform::Hashing::Internal::AnyHashers[object.type()];
        return hasher(object);
    }
};

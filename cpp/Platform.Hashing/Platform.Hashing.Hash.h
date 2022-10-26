#pragma once

#include <functional>
#include <cstring>
#include <ranges>

namespace Platform::Hashing
{

    namespace Internal
    {
        template <typename Self>
        concept std_hashable = requires(Self self) { std::hash<Self>{}(self); };

        template <typename Self>
        concept not_std_hashable = not requires(Self self) { std::hash<Self>{}(self); };

        template <typename RawSelf, typename Self = std::remove_cvref_t<RawSelf>>
        concept char_type =
            std::same_as<Self, char> ||
            std::same_as<Self, char8_t> ||
            std::same_as<Self, char16_t> ||
            std::same_as<Self, char32_t> ||
            std::same_as<Self, wchar_t>;

        template <typename RawSelf,
            typename Self = std::decay_t<RawSelf>>
        concept char_array = requires(std::ranges::range_value_t<RawSelf> item)
        {
            requires std::same_as<Self, const decltype(item)*>;
            requires char_type<decltype(item)>;
        };

        template <typename Self>
        concept smart_hashable =
            std::is_standard_layout_v<Self>// &&
            // std::is_trivial_v<Self>
             or
            std::is_fundamental_v<Self>;
    }

    template<typename T> std::size_t HashRaw(const T &value, std::size_t size = 1)
    {
        std::size_t hash = typeid(T).hash_code();
        Combine(hash, &value, size);
        return hash;
    }

    std::size_t Hash(auto&& value)
        requires
            Internal::char_array<decltype(value)>
             or
            Internal::std_hashable<std::remove_cvref_t<decltype(value)>>
             or
            Internal::smart_hashable<std::remove_cvref_t<decltype(value)>>
    {
        using RawType = decltype(value);
        using Type = std::remove_cvref_t<decltype(value)>;

        if constexpr (Internal::char_array<RawType>)
        {
            std::hash<const std::ranges::range_value_t<Type>*> hasher;
            return hasher(value);
        }

        if constexpr (Internal::std_hashable<Type>)
        {
            std::hash<Type> hasher;
            return hasher(value);
        }
        else
        {
            return HashRaw(value);
        }
    }

    std::size_t Hash(auto&&... args) requires (sizeof...(args) > 1)
    {
        std::hash<decltype(std::tie(args...))> hasher;
        return hasher(std::tie(args...));
    }
}

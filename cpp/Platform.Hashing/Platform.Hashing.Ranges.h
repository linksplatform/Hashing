#pragma once

#include "Platform.Hashing.Hash.h"

namespace std
{
    template<Platform::Hashing::Internal::not_std_hashable Self>
        requires
            requires(Self collection)
            {
                std::ranges::data(collection);
                std::ranges::size(collection);
            }
    struct hash<Self>
    {
        std::size_t operator()(const Self& collection) const
        {
            using namespace Platform::Hashing;
            using TItem = std::ranges::range_value_t<Self>;

            std::size_t hash = 0;
            if constexpr (is_fundamental_v<TItem>)
            {
                Combine(hash, std::ranges::data(collection), std::ranges::size(collection));
                return hash;
            }
            else
            {
                auto data = std::ranges::data(collection);
                auto size = std::ranges::size(collection);
                for (int i = 0; i < size; i++)
                {
                    hash = CombineHashes(hash, Hash(*data));
                    ++data;
                }
                return hash;
            }
        }
    };

    template<Platform::Hashing::Internal::not_std_hashable Self>
    requires
        (not requires(Self collection)
        {
            std::ranges::data(collection);
            std::ranges::size(collection);
        })
        &&
        std::ranges::range<Self>
    struct hash<Self>
    {
        std::size_t operator()(const Self& collection) const
        {
            using TItem = std::ranges::range_value_t<Self>;

            std::size_t hash = 0;
            std::size_t size = 0;
            std::vector<TItem> data;

            constexpr auto sizeable = requires
            {
                std::ranges::size(collection);
            };

            if constexpr (sizeable)
            {
                data.reserve(std::ranges::size(collection));
            }

            for (auto&& it : collection)
            {
                data.push_back(it);
            }

            return Platform::Hashing::Hash(data);
        }
    };
}

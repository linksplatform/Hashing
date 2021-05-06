#ifndef PLATFORM_HASHING_RANGE
#define PLATFORM_HASHING_RANGE

#include "Platform.Hashing.Hash.h"

namespace std
{
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
            using namespace Platform::Hashing;
            using TItem = std::ranges::range_value_t<T>;
            std::uint32_t hash = 0;

            if constexpr (is_fundamental_v<TItem>)
            {
                Combine(hash, std::ranges::data(collection), std::ranges::size(collection));
                return Expand(hash);
            }
            else
            {
                auto data = std::ranges::data(collection);
                const auto size = std::ranges::size(collection);
                for (int i = 0; i < size; i++)
                {
                    hash = CombineHashes(hash, Hash(*data));
                    ++data;
                }
                return Expand(hash);
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

            constexpr auto sizeable = requires()
            {
                std::ranges::size(collection);
            };

            if constexpr (sizeable)
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

#endif //PLATFORM_HASHING_RANGE

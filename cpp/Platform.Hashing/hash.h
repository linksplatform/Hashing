#pragma once

#include <cstring>
#include <functional>
#include <iostream>
#include <ranges>

namespace Platform::Hashing {

// fixme: at now `CrcHash` (DefaultHash) must be declared after `hash`/`hash_span`
template <typename T>
struct CrcHash {};

template <typename T, typename Hasher>
struct hash_span_fn {
    template <size_t N>
    // `std::span<const T>` is very stupid
    auto operator()(std::span</*const */ T, N> span) const -> uint64_t {
        uint64_t accum = 0;
        for (auto&& piece : span) {
            combine_hashes(accum, Hasher{}(piece));
        }
        return accum;
    }
};

template <typename T, template <typename> typename Hasher = CrcHash>
constexpr auto hash(const T& val) noexcept(noexcept(Hasher<T>{}(val))) -> uint64_t {
    return Hasher<T>{}(val);
}

template <std::size_t... Idx>
auto combine_tuple(const auto& tp, std::index_sequence<Idx...> /*unused*/) noexcept -> uint64_t {
    uint64_t accum = 0;
    ((combine_to(accum, hash(std::get<Idx>(tp)))), ...);
    return accum;
}

template <template <typename> typename Hasher = CrcHash>
constexpr auto hash(const auto&... args) noexcept -> uint64_t {
    return combine_tuple(std::tie(args...), std::make_index_sequence<sizeof...(args)>{});
}

template <typename T, typename Hasher = CrcHash<std::decay_t<T>>, size_t N>
constexpr auto hash_span(std::span<T, N> span) noexcept -> uint64_t {
    return hash_span_fn<T, Hasher>{}(span);
}

}  // namespace Platform::Hashing

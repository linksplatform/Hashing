#pragma once

namespace Platform::Hashing {


template <noexport::std_hashable T>
struct CrcHash<T> {
    auto operator()(const T& val) const -> uint64_t { return std::hash<T>{}(val); }
};

template <typename Self>
concept crc_hashable = requires(Self self) { CrcHash<Self>{}(self); };

template <typename Self>
concept not_crc_hashable = not requires(Self self) { CrcHash<Self>{}(self); };

template <noexport::trivial_hashable T>
    requires noexport::not_std_hashable<T> && not_crc_hashable<T>
struct CrcHash<T> {
    auto operator()(const T& val) const -> uint64_t {
        uint64_t accum = 0;
        combine_crc32(accum, val);
        return accum;
    }
};

template <crc_hashable... T>
struct CrcHash<std::tuple<T...>> {
    auto operator()(const std::tuple<T...>& tuple) const -> uint64_t {
        // fixme: add case where `T` is `trivially_hashable`
        return std::apply([](auto&&... args) { return hash(args...); }, tuple);
    }
};

template <noexport::trivial_hashable T>
struct hash_span_fn<T, CrcHash<std::decay_t<T>>> {
    template <size_t N>
    auto operator()(std::span<T, N> span) const -> uint64_t {
        uint64_t accum = 0;
        auto bytes = std::as_bytes(span);
        combine_crc32_unsafe(accum, bytes.data(), bytes.size());
        return accum;
    }
};

}  // namespace Platform::Hashing
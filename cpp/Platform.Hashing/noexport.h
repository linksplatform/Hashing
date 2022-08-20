#pragma once

// clang-format off
// TTA == template <template> argument (just for better reading)
#define TTA template <typename> typename
// clang-format on

namespace noexport {

template <typename Self>
concept std_hashable = requires(Self self) { std::hash<Self>{}(self); };

template <typename Self>
concept not_std_hashable = not requires(Self self) { std::hash<Self>{}(self); };

template <TTA Hasher, typename... T>
constexpr bool noexcept_with = noexcept((Hasher<std::remove_cvref_t<T>>{}(std::declval<T>()), ...));

}  // namespace noexport

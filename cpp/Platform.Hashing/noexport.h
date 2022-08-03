#pragma once

namespace noexport {
template <typename Self>
concept std_hashable = requires(Self self) { std::hash<Self>{}(self); };

template <typename Self>
concept not_std_hashable = not requires(Self self) { std::hash<Self>{}(self); };
}  // namespace noexport

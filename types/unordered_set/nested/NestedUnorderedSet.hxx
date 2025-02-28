#pragma once

#include <cstdint>
#include <unordered_set>

template <> struct std::hash<std::unordered_set<std::int32_t>> {
  std::size_t
  operator()(const std::unordered_set<std::int32_t> &s) const noexcept {
    std::size_t h = 0;
    for (const auto &el : s) {
      h ^= std::hash<std::int32_t>{}(el);
    }
    return h;
  }
};

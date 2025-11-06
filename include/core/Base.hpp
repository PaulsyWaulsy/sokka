#pragma once

#include <memory>
namespace Sokka {
template <typename T>
using Unique = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Unique<T> makeUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Shared = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Shared<T> makeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
}  // namespace Sokka

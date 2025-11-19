#pragma once

// Marks a class as move-only: no copying, defaulted moves.
#define MARK_CLASS_MOVEONLY(Type)                                              \
  Type(const Type &) = delete;                                                 \
  Type &operator=(const Type &) = delete;                                      \
  Type(Type &&) noexcept = default;                                            \
  Type &operator=(Type &&) noexcept = default

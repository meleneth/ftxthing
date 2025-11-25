#pragma once
#define MARK_IMMOVABLE(Type)                                                   \
  Type(const Type &) = delete;                                                 \
  Type &operator=(const Type &) = delete;                                      \
  Type(Type &&) = delete;                                                      \
  Type &operator=(Type &&) = delete;

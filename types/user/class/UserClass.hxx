#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

using Variant = std::variant<std::int32_t, std::string>;
using Vector = std::vector<std::int32_t>;

class Base {
public:
  std::int32_t fInt;
};

class Nested {
public:
  std::int32_t fInt;
};

class User : public Base {
public:
  std::string fString;
  Variant fVariant;
  Vector fVector;
  Nested fNested;
};

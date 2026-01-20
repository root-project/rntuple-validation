#include <cstdint>
#include <string>
#include <variant>

#ifdef __CLING__
#pragma link C++ class Base+;
#pragma link C++ class Nested+;
// ROOT does not (yet) support std::variant, but this line silences a warning
// that fVariant would not be saved (which is not true for RNTuple).
#pragma link C++ class std::variant<std::int32_t, std::string>;
#pragma link C++ class User+;
#endif

#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

using Pair_Int32_String = std::pair<std::int32_t, std::string>;
using Variant = std::variant<std::int32_t, std::string>;
using VectorInt32 = std::vector<std::int32_t>;

template <typename T> static void PrintValue(const T &value, std::ostream &os);

template <> void PrintValue(const std::int32_t &value, std::ostream &os) {
  os << value;
}

template <> void PrintValue(const std::string &value, std::ostream &os) {
  os << "\"" << value << "\"";
}

template <> void PrintValue(const Variant &value, std::ostream &os) {
  if (value.index() == 0) {
    PrintValue(std::get<std::int32_t>(value), os);
  } else if (value.index() == 1) {
    PrintValue(std::get<std::string>(value), os);
  }
}

template <> void PrintValue(const VectorInt32 &value, std::ostream &os) {
  os << "[";
  bool first = true;
  for (auto element : value) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n        " << element;
  }
  if (!value.empty()) {
    os << "\n      ";
  }
  os << "]";
}

template <> void PrintValue(const Pair_Int32_String &value, std::ostream &os) {
  os << "[\n";
  os << "        " << value.first << ",\n";
  os << "        \"" << value.second << "\"\n";
  os << "      ]";
}

template <typename T, typename U>
static void PrintPairValue(const REntry &entry, std::string_view name,
                           std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<std::pair<T, U>>(name);
  os << "    \"" << name << "\": [\n";
  os << "      ";
  PrintValue(value.first, os);
  os << ",\n";
  os << "      ";
  PrintValue(value.second, os);
  os << "\n";
  os << "    ]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

static void PrintVectorPairValue(const REntry &entry, std::string_view name,
                                 std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<std::vector<Pair_Int32_String>>(name);
  os << "    \"" << name << "\": [";
  bool first = true;
  for (auto &&element : value) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n      ";
    PrintValue(element, os);
  }
  if (!value.empty()) {
    os << "\n    ";
  }
  os << "]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.pair.root",
          std::string_view output = "types.pair.json") {
  std::ofstream os(std::string{output});
  os << "[\n";

  auto reader = RNTupleReader::Open("ntpl", input);
  auto &entry = reader->GetModel().GetDefaultEntry();
  bool first = true;
  for (auto index : *reader) {
    reader->LoadEntry(index);

    if (first) {
      first = false;
    } else {
      os << ",\n";
    }
    os << "  {\n";

    PrintPairValue<std::int32_t, std::string>(entry, "Int32_String", os);
    PrintPairValue<Variant, VectorInt32>(entry, "Variant_Vector", os);
    PrintPairValue<Pair_Int32_String, std::int32_t>(entry, "Pair", os);
    PrintVectorPairValue(entry, "VectorPair", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

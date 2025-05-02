#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

using Tuple_Int32_String = std::tuple<std::int32_t, std::string>;
using VectorInt32 = std::vector<std::int32_t>;

template <typename T> static void PrintValue(const T &value, std::ostream &os);

template <> void PrintValue(const std::int32_t &value, std::ostream &os) {
  os << value;
}

template <> void PrintValue(const std::string &value, std::ostream &os) {
  os << "\"" << value << "\"";
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

template <> void PrintValue(const Tuple_Int32_String &value, std::ostream &os) {
  os << "[\n";
  os << "        " << std::get<0>(value) << ",\n";
  os << "        \"" << std::get<1>(value) << "\"\n";
  os << "      ]";
}

static void PrintTupleValue(const REntry &entry, std::string_view name,
                            std::ostream &os, bool last = false) {
  auto &value =
      *entry.GetPtr<std::tuple<std::int32_t, std::string, VectorInt32>>(name);
  os << "    \"" << name << "\": [\n";
  os << "      ";
  PrintValue(std::get<0>(value), os);
  os << ",\n";
  os << "      ";
  PrintValue(std::get<1>(value), os);
  os << ",\n";
  os << "      ";
  PrintValue(std::get<2>(value), os);
  os << "\n";
  os << "    ]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

static void PrintVariantValue(const REntry &entry, std::string_view name,
                              std::ostream &os, bool last = false) {
  auto &value =
      *entry.GetPtr<std::tuple<std::variant<std::int32_t, std::string>>>(name);
  os << "    \"" << name << "\": [\n";
  os << "      ";
  auto &variant = std::get<0>(value);
  if (variant.index() == 0) {
    PrintValue(std::get<std::int32_t>(variant), os);
  } else if (variant.index() == 1) {
    PrintValue(std::get<std::string>(variant), os);
  }
  os << "\n";
  os << "    ]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

static void PrintNestedValue(const REntry &entry, std::string_view name,
                             std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<std::tuple<Tuple_Int32_String>>(name);
  os << "    \"" << name << "\": [\n";
  os << "      ";
  PrintValue(std::get<0>(value), os);
  os << "\n";
  os << "    ]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

static void PrintVectorTupleValue(const REntry &entry, std::string_view name,
                                  std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<std::vector<Tuple_Int32_String>>(name);
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

void read(std::string_view input = "types.tuple.root",
          std::string_view output = "types.tuple.json") {
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

    PrintTupleValue(entry, "Int32_String_Vector", os);
    PrintVariantValue(entry, "Variant", os);
    PrintNestedValue(entry, "Tuple", os);
    PrintVectorTupleValue(entry, "VectorTuple", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

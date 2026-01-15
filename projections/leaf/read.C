#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>
#include <ROOT/RNTupleUtil.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

template <typename T> static void PrintValue(const T &value, std::ostream &os);

template <> void PrintValue(const std::int32_t &value, std::ostream &os) {
  os << value;
}

template <> void PrintValue(const float &value, std::ostream &os) {
  os << "\"" << value << "\"";
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

template <typename T>
static void PrintValue(const REntry &entry, std::string_view name,
                       std::ostream &os, bool last = false) {
  T value = *entry.GetPtr<T>(name);
  os << "    \"" << name << "\": ";
  PrintValue(value, os);
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "projections.leaf.root",
          std::string_view output = "projections.leaf.json") {
  std::ofstream os(std::string{output});
  // Print floating-point numbers as hexadecimal literals.
  os << std::hexfloat;
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

    PrintPairValue<std::int32_t, float>(entry, "Pair", os);
    PrintValue<std::int32_t>(entry, "Int32", os);
    PrintValue<float>(entry, "Float", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

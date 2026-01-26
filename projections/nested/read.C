#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>
#include <ROOT/RVec.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

template <typename T> static void PrintValue(const T &value, std::ostream &os);

template <> void PrintValue(const std::int32_t &value, std::ostream &os) {
  os << value;
}

template <> void PrintValue(const float &value, std::ostream &os) {
  os << "\"" << value << "\"";
}

template <>
void PrintValue(const std::pair<std::int32_t, float> &value, std::ostream &os) {
  os << "[\n";
  os << "        ";
  PrintValue(value.first, os);
  os << ",\n";
  os << "        ";
  PrintValue(value.second, os);
  os << "\n";
  os << "      ]";
}

template <typename T>
static void PrintVectorValue(const REntry &entry, std::string_view name,
                             std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<std::vector<T>>(name);
  os << "    \"" << name << "\": [";
  bool first = true;
  for (auto element : value) {
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

void read(std::string_view input = "projections.nested.root",
          std::string_view output = "projections.nested.json") {
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

    PrintVectorValue<std::pair<std::int32_t, float>>(entry, "VectorPair", os);
    PrintVectorValue<std::int32_t>(entry, "VectorInt", os);
    PrintVectorValue<float>(entry, "VectorFloat", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

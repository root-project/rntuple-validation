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
#include <vector>

using Vector = std::vector<std::int32_t>;

static void PrintVectorValue(const REntry &entry, std::string_view name,
                             std::ostream &os, bool last = false) {
  Vector &value = *entry.GetPtr<Vector>(name);
  os << "    \"" << name << "\": [";
  bool first = true;
  for (auto element : value) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n      " << element;
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

template <typename T>
static void PrintCardinality(const REntry &entry, std::string_view name,
                             std::ostream &os, bool last = false) {
  T value = *entry.GetPtr<ROOT::RNTupleCardinality<T>>(name);
  os << "    \"" << name << "\": " << value;
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "projections.cardinality.root",
          std::string_view output = "projections.cardinality.json") {
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

    PrintVectorValue(entry, "Index32", os);
    PrintCardinality<std::uint32_t>(entry, "Index32Cardinality", os);
    PrintVectorValue(entry, "Index64", os);
    PrintCardinality<std::uint64_t>(entry, "Index64Cardinality", os);
    PrintVectorValue(entry, "SplitIndex32", os);
    PrintCardinality<std::uint32_t>(entry, "SplitIndex32Cardinality", os);
    PrintVectorValue(entry, "SplitIndex64", os);
    PrintCardinality<std::uint64_t>(entry, "SplitIndex64Cardinality", os,
                                    /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

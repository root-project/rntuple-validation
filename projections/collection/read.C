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
#include <vector>

template <typename C>
static void PrintCollectionValue(const REntry &entry, std::string_view name,
                                 std::ostream &os, bool last = false) {
  C &value = *entry.GetPtr<C>(name);
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

void read(std::string_view input = "projections.collection.root",
          std::string_view output = "projections.collection.json") {
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

    PrintCollectionValue<std::vector<std::int32_t>>(entry, "Vector", os);
    PrintCollectionValue<ROOT::RVec<std::int32_t>>(entry, "Projected", os,
                                                   /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

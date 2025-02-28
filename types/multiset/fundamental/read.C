#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <set>
#include <string>
#include <string_view>

using Multiset = std::multiset<std::int32_t>;

static void PrintMultisetValue(const REntry &entry, std::string_view name,
                               std::ostream &os, bool last = false) {
  Multiset &value = *entry.GetPtr<Multiset>(name);
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

void read(std::string_view input = "types.multiset.fundamental.root",
          std::string_view output = "types.multiset.fundamental.json") {
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

    PrintMultisetValue(entry, "Index32", os);
    PrintMultisetValue(entry, "Index64", os);
    PrintMultisetValue(entry, "SplitIndex32", os);
    PrintMultisetValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

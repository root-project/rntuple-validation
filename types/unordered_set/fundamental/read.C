#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_set>

using UnorderedSet = std::unordered_set<std::int32_t>;

static void PrintUnorderedSetValue(const REntry &entry, std::string_view name,
                                   std::ostream &os, bool last = false) {
  UnorderedSet &value = *entry.GetPtr<UnorderedSet>(name);

  std::vector<UnorderedSet::value_type> valueSorted(value.begin(), value.end());
  std::sort(valueSorted.begin(), valueSorted.end());

  os << "    \"" << name << "\": [";
  bool first = true;
  for (auto element : valueSorted) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n      " << element;
  }
  if (!valueSorted.empty()) {
    os << "\n    ";
  }
  os << "]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.unordered_set.fundamental.root",
          std::string_view output = "types.unordered_set.fundamental.json") {
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

    PrintUnorderedSetValue(entry, "Index32", os);
    PrintUnorderedSetValue(entry, "Index64", os);
    PrintUnorderedSetValue(entry, "SplitIndex32", os);
    PrintUnorderedSetValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

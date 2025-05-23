#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <TSystem.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_set>

using UnorderedSet = std::unordered_set<std::unordered_set<std::int32_t>>;

static void PrintNestedUnorderedSetValue(const REntry &entry,
                                         std::string_view name,
                                         std::ostream &os, bool last = false) {
  UnorderedSet &value = *entry.GetPtr<UnorderedSet>(name);

  std::vector<std::vector<UnorderedSet::value_type::value_type>> valueSorted;

  for (auto inner : value) {
    std::vector innerSorted(inner.begin(), inner.end());
    std::sort(innerSorted.begin(), innerSorted.end());
    valueSorted.push_back(innerSorted);
  }

  std::sort(valueSorted.begin(), valueSorted.end());

  os << "    \"" << name << "\": [";
  bool outerFirst = true;
  for (auto inner : valueSorted) {
    if (outerFirst) {
      outerFirst = false;
    } else {
      os << ",";
    }
    os << "\n      [";
    bool innerFirst = true;
    for (auto element : inner) {
      if (innerFirst) {
        innerFirst = false;
      } else {
        os << ",";
      }
      os << "\n        " << element;
    }
    if (!inner.empty()) {
      os << "\n      ";
    }
    os << "]";
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

void read(std::string_view input = "types.unordered_set.nested.root",
          std::string_view output = "types.unordered_set.nested.json") {
  if (gSystem->Load("libNestedUnorderedSet") == -1)
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");

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

    PrintNestedUnorderedSetValue(entry, "Index32", os);
    PrintNestedUnorderedSetValue(entry, "Index64", os);
    PrintNestedUnorderedSetValue(entry, "SplitIndex32", os);
    PrintNestedUnorderedSetValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

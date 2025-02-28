#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <TSystem.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <set>
#include <string>
#include <string_view>

using Multiset = std::multiset<std::multiset<std::int32_t>>;

static void PrintNestedMultisetValue(const REntry &entry, std::string_view name,
                                     std::ostream &os, bool last = false) {
  Multiset &value = *entry.GetPtr<Multiset>(name);
  os << "    \"" << name << "\": [";
  bool outerFirst = true;
  for (auto inner : value) {
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
  if (!value.empty()) {
    os << "\n    ";
  }
  os << "]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.multiset.nested.root",
          std::string_view output = "types.multiset.nested.json") {
  if (!std::filesystem::exists("libNestedMultiset.so")) {
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");
  }

  gSystem->Load("libNestedMultiset");
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

    PrintNestedMultisetValue(entry, "Index32", os);
    PrintNestedMultisetValue(entry, "Index64", os);
    PrintNestedMultisetValue(entry, "SplitIndex32", os);
    PrintNestedMultisetValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

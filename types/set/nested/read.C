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

using Set = std::set<std::set<std::int32_t>>;

static void PrintNestedSetValue(const REntry &entry, std::string_view name,
                                std::ostream &os, bool last = false) {
  Set &value = *entry.GetPtr<Set>(name);
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

void read(std::string_view input = "types.set.nested.root",
          std::string_view output = "types.set.nested.json") {
  if (gSystem->Load("libNestedSet") == -1)
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

    PrintNestedSetValue(entry, "Index32", os);
    PrintNestedSetValue(entry, "Index64", os);
    PrintNestedSetValue(entry, "SplitIndex32", os);
    PrintNestedSetValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

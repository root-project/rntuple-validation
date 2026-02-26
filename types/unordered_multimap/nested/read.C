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
#include <unordered_map>

using UnorderedMultimap =
    std::unordered_multimap<std::string,
                       std::unordered_multimap<std::string, std::int32_t>>;

static void PrintNestedUnorderedMultimapValue(const REntry &entry,
                                         std::string_view name,
                                         std::ostream &os, bool last = false) {
  UnorderedMultimap &item = *entry.GetPtr<UnorderedMultimap>(name);
  os << "    \"" << name << "\": {";
  bool outerFirst = true;
  for (auto &[key, inner] : item) {
    if (outerFirst) {
      outerFirst = false;
    } else {
      os << ",";
    }
    os << "\n      \"" << key << "\": {";
    bool innerFirst = true;
    for (auto &[innerKey, value] : inner) {
      if (innerFirst) {
        innerFirst = false;
      } else {
        os << ",";
      }
      os << "\n        \"" << innerKey << "\": " << value;
    }
    if (!inner.empty()) {
      os << "\n      ";
    }
    os << "}";
  }
  if (!item.empty()) {
    os << "\n    ";
  }
  os << "}";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.unordered_multimap.nested.root",
          std::string_view output = "types.unordered_multimap.nested.json") {
  if (gSystem->Load("libNestedUnorderedMultimap") == -1)
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

    PrintNestedUnorderedMultimapValue(entry, "Index32", os);
    PrintNestedUnorderedMultimapValue(entry, "Index64", os);
    PrintNestedUnorderedMultimapValue(entry, "SplitIndex32", os);
    PrintNestedUnorderedMultimapValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

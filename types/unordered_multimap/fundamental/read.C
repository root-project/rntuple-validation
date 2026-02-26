#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>

using UnorderedMultimap = std::unordered_multimap<std::string, std::int32_t>;

static void PrintMultimapValue(const REntry &entry, std::string_view name,
                          std::ostream &os, bool last = false) {
  UnorderedMultimap &item = *entry.GetPtr<UnorderedMultimap>(name);
  os << "    \"" << name << "\": {";
  bool first = true;
  for (auto &[key, value] : item) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n      \"" << key << "\": " << value;
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

void read(std::string_view input = "types.unordered_multimap.fundamental.root",
          std::string_view output = "types.unordered_multimap.fundamental.json") {
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

    PrintMultimapValue(entry, "Index32", os);
    PrintMultimapValue(entry, "Index64", os);
    PrintMultimapValue(entry, "SplitIndex32", os);
    PrintMultimapValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <map>
#include <string>
#include <string_view>

using Map = std::map<std::string, std::int32_t>;

static void PrintMapValue(const REntry &entry, std::string_view name,
                          std::ostream &os, bool last = false) {
  Map &item = *entry.GetPtr<Map>(name);
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

void read(std::string_view input = "types.map.fundamental.root",
          std::string_view output = "types.map.fundamental.json") {
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

    PrintMapValue(entry, "Index32", os);
    PrintMapValue(entry, "Index64", os);
    PrintMapValue(entry, "SplitIndex32", os);
    PrintMapValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

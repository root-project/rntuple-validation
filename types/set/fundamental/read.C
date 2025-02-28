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

using Set = std::set<std::int32_t>;

static void PrintSetValue(const REntry &entry, std::string_view name,
                          std::ostream &os, bool last = false) {
  Set &value = *entry.GetPtr<Set>(name);
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

void read(std::string_view input = "types.set.fundamental.root",
          std::string_view output = "types.set.fundamental.json") {
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

    PrintSetValue(entry, "Index32", os);
    PrintSetValue(entry, "Index64", os);
    PrintSetValue(entry, "SplitIndex32", os);
    PrintSetValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

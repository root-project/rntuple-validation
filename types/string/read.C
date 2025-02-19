#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <fstream>
#include <ostream>
#include <string>
#include <string_view>

static void PrintStringValue(const REntry &entry, std::string_view name,
                             std::ostream &os, bool last = false) {
  std::string &value = *entry.GetPtr<std::string>(name);
  os << "    \"" << name << "\": \"" << value << "\"";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.string.root",
          std::string_view output = "types.string.json") {
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

    PrintStringValue(entry, "Index32", os);
    PrintStringValue(entry, "Index64", os);
    PrintStringValue(entry, "SplitIndex32", os);
    PrintStringValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

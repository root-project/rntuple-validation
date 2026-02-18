#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <bitset>
#include <cstddef>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>

template <std::size_t N>
static void PrintBitsetValue(const REntry &entry, std::string_view name,
                             std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<std::bitset<N>>(name);
  os << "    \"" << name << "\": \"" << value.to_string() << "\"";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.bitset.root",
          std::string_view output = "types.bitset.json") {
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

    PrintBitsetValue<1>(entry, "Bitset1", os);
    PrintBitsetValue<64>(entry, "Bitset64", os);
    PrintBitsetValue<65>(entry, "Bitset65", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

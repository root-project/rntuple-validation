#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstddef> // for std::byte
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>

template <typename T>
static void PrintIntegerValue(const REntry &entry, std::string_view name,
                              std::ostream &os, bool last = false) {
  T value = *entry.GetPtr<T>(name);
  os << "    \"" << name << "\": ";
  // We want to print the integer value even if it is a character; use the unary
  // + operator (https://stackoverflow.com/a/28414758).
  os << +value;
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.fundamental.misc.root",
          std::string_view output = "types.fundamental.misc.json") {
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

    PrintIntegerValue<bool>(entry, "Bit", os);
    os << "    \"Byte\": ";
    os << std::to_integer<int>(*entry.GetPtr<std::byte>("Byte"));
    os << ",\n";
    PrintIntegerValue<char>(entry, "Char", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

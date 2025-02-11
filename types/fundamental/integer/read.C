#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
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

void read(std::string_view input = "types.fundamental.integer.root",
          std::string_view output = "types.fundamental.integer.json") {
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

    PrintIntegerValue<std::int8_t>(entry, "Int8", os);
    PrintIntegerValue<std::uint8_t>(entry, "UInt8", os);
    PrintIntegerValue<std::int16_t>(entry, "Int16", os);
    PrintIntegerValue<std::uint16_t>(entry, "UInt16", os);
    PrintIntegerValue<std::int32_t>(entry, "Int32", os);
    PrintIntegerValue<std::uint32_t>(entry, "UInt32", os);
    PrintIntegerValue<std::int64_t>(entry, "Int64", os);
    PrintIntegerValue<std::uint64_t>(entry, "UInt64", os);
    PrintIntegerValue<std::int16_t>(entry, "SplitInt16", os);
    PrintIntegerValue<std::uint16_t>(entry, "SplitUInt16", os);
    PrintIntegerValue<std::int32_t>(entry, "SplitInt32", os);
    PrintIntegerValue<std::uint32_t>(entry, "SplitUInt32", os);
    PrintIntegerValue<std::int64_t>(entry, "SplitInt64", os);
    PrintIntegerValue<std::uint64_t>(entry, "SplitUInt64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

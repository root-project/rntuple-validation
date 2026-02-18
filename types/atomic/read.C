#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <atomic>
#include <cstddef> // for std::byte
#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>

template <typename T>
static void PrintIntegerValue(const REntry &entry, std::string_view name,
                              std::ostream &os, bool last = false) {
  T value = *entry.GetPtr<std::atomic<T>>(name);
  os << "    \"" << name << "\": ";
  // We want to print the integer value even if it is a character; use the unary
  // + operator (https://stackoverflow.com/a/28414758).
  os << +value;
  if (!last) {
    os << ",";
  }
  os << "\n";
}

template <typename T>
static void PrintRealValue(const REntry &entry, std::string_view name,
                           std::ostream &os, bool last = false) {
  T value = *entry.GetPtr<std::atomic<T>>(name);
  os << "    \"" << name << "\": \"" << value << "\"";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.atomic.root",
          std::string_view output = "types.atomic.json") {
  std::ofstream os(std::string{output});
  // Print floating-point numbers as hexadecimal literals.
  os << std::hexfloat;
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
    os << std::to_integer<int>(*entry.GetPtr<std::atomic<std::byte>>("Byte"));
    os << ",\n";
    PrintIntegerValue<char>(entry, "Char", os);
    PrintIntegerValue<std::int8_t>(entry, "Int8", os);
    PrintIntegerValue<std::uint8_t>(entry, "UInt8", os);
    PrintIntegerValue<std::int16_t>(entry, "Int16", os);
    PrintIntegerValue<std::uint16_t>(entry, "UInt16", os);
    PrintIntegerValue<std::int32_t>(entry, "Int32", os);
    PrintIntegerValue<std::uint32_t>(entry, "UInt32", os);
    PrintIntegerValue<std::int64_t>(entry, "Int64", os);
    PrintIntegerValue<std::uint64_t>(entry, "UInt64", os);
    PrintRealValue<float>(entry, "Real32", os);
    PrintRealValue<double>(entry, "Real64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

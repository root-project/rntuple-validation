#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>

#include "UserEnum.hxx"

template <typename T>
static void PrintEnumValue(const REntry &entry, std::string_view name,
                           std::ostream &os, bool last = false) {
  T value = *entry.GetPtr<T>(name);
  os << "    \"" << name << "\": ";
  // We want to print the integer value even if it is a character; use the unary
  // + operator (https://stackoverflow.com/a/28414758).
  os << +static_cast<std::underlying_type_t<T>>(value);
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.user.enum.root",
          std::string_view output = "types.user.enum.json") {
  if (gSystem->Load("libUserEnum") == -1)
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

    PrintEnumValue<EnumInt8>(entry, "EnumInt8", os);
    PrintEnumValue<EnumUInt8>(entry, "EnumUInt8", os);
    PrintEnumValue<EnumInt16>(entry, "EnumInt16", os);
    PrintEnumValue<EnumUInt16>(entry, "EnumUInt16", os);
    PrintEnumValue<EnumInt32>(entry, "EnumInt32", os);
    PrintEnumValue<EnumUInt32>(entry, "EnumUInt32", os);
    PrintEnumValue<EnumInt64>(entry, "EnumInt64", os);
    PrintEnumValue<EnumUInt64>(entry, "EnumUInt64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

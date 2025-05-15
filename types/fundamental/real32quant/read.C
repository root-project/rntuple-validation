#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <fstream>
#include <ios>
#include <ostream>
#include <string>
#include <string_view>

template <typename T>
static void PrintRealValue(const REntry &entry, std::string_view name,
                           std::ostream &os, bool last = false) {
  T value = *entry.GetPtr<T>(name);
  os << "    \"" << name << "\": \"" << value << "\"";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.fundamental.real32quant.root",
          std::string_view output = "types.fundamental.real32quant.json") {
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

    PrintRealValue<float>(entry, "FloatReal32Quant1", os);
    PrintRealValue<float>(entry, "FloatReal32Quant8", os);
    PrintRealValue<float>(entry, "FloatReal32Quant32", os);
    PrintRealValue<double>(entry, "DoubleReal32Quant1", os);
    PrintRealValue<double>(entry, "DoubleReal32Quant20", os);
    PrintRealValue<double>(entry, "DoubleReal32Quant32", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

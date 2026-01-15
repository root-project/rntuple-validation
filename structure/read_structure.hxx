#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>

void read_structure(std::string_view input, std::string_view output) {
  std::ofstream os(std::string{output});
  os << "[\n";

  auto reader = RNTupleReader::Open("ntpl", input);
  auto Int32 =
      reader->GetModel().GetDefaultEntry().GetPtr<std::int32_t>("Int32");
  bool first = true;
  for (auto index : *reader) {
    reader->LoadEntry(index);

    if (first) {
      first = false;
    } else {
      os << ",\n";
    }
    os << "  {\n";
    os << "    \"Int32\": " << *Int32 << "\n";
    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  if (!first) {
    os << "\n";
  }
  os << "]\n";
}

#include <ROOT/REntry.hxx>
#include <ROOT/RError.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::RException;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>

void read_structure(std::string_view input, std::string_view output) {
  std::ofstream os(std::string{output});

  std::unique_ptr<RNTupleReader> reader;
  try {
    reader = RNTupleReader::Open("ntpl", input);
  } catch (const RException &e) {
    std::string msgWithoutStacktrace;
    std::getline(std::istringstream(e.what()), msgWithoutStacktrace);
    os << "{\n";
    os << "  \"error\": \"" << msgWithoutStacktrace << "\"\n";
    os << "}\n";
    return;
  }

  os << "[\n";
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

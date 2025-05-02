#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>

void read_compression(std::string_view input, std::string_view output) {
  auto reader = RNTupleReader::Open("ntpl", input);
  auto Int64 =
      reader->GetModel().GetDefaultEntry().GetPtr<std::int64_t>("Int64");
  std::int64_t sum = 0;
  for (auto index : *reader) {
    reader->LoadEntry(index);
    sum += *Int64;
  }

  std::ofstream os(std::string{output});
  os << "{\n";
  os << "  \"Int64\": " << sum << "\n";
  os << "}\n";
}

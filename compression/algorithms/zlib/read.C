#include "../../read_compression.hxx"

void read(std::string_view input = "compression.algorithms.zlib.root",
          std::string_view output = "compression.algorithms.zlib.json") {
  read_compression(input, output);
}

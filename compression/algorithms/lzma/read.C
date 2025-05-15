#include "../../read_compression.hxx"

void read(std::string_view input = "compression.algorithms.lzma.root",
          std::string_view output = "compression.algorithms.lzma.json") {
  read_compression(input, output);
}

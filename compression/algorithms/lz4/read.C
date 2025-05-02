#include "../../read_compression.hxx"

void read(std::string_view input = "compression.algorithms.lz4.root",
          std::string_view output = "compression.algorithms.lz4.json") {
  read_compression(input, output);
}

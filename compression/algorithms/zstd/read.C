#include "../../read_compression.hxx"

void read(std::string_view input = "compression.algorithms.zstd.root",
          std::string_view output = "compression.algorithms.zstd.json") {
  read_compression(input, output);
}

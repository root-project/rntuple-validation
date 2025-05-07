#include "../../read_compression.hxx"

void read(std::string_view input = "compression.block.big.root",
          std::string_view output = "compression.block.big.json") {
  read_compression(input, output);
}

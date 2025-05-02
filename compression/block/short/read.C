#include "../../read_compression.hxx"

void read(std::string_view input = "compression.block.short.root",
          std::string_view output = "compression.block.short.json") {
  read_compression(input, output);
}

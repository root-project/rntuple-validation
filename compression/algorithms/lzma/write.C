#include "../write_algorithm.hxx"

void write(std::string_view filename = "compression.algorithms.lzma.root") {
  write_algorithm(filename, 207);
}

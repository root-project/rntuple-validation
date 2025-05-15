#include "../write_algorithm.hxx"

void write(std::string_view filename = "compression.algorithms.lz4.root") {
  write_algorithm(filename, 404);
}

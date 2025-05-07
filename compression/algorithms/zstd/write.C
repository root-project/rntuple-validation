#include "../write_algorithm.hxx"

void write(std::string_view filename = "compression.algorithms.zstd.root") {
  write_algorithm(filename, 505);
}

#include "../write_algorithm.hxx"

void write(std::string_view filename = "compression.algorithms.zlib.root") {
  write_algorithm(filename, 101);
}

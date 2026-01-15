#include "../read_structure.hxx"

void read(std::string_view input = "structure.clusters.root",
          std::string_view output = "structure.clusters.json") {
  read_structure(input, output);
}

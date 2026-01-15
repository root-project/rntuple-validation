#include "../read_structure.hxx"

void read(std::string_view input = "structure.cluster_groups.root",
          std::string_view output = "structure.cluster_groups.json") {
  read_structure(input, output);
}

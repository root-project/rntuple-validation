#include "../read_structure.hxx"

void read(std::string_view input = "structure.empty.root",
          std::string_view output = "structure.empty.json") {
  read_structure(input, output);
}

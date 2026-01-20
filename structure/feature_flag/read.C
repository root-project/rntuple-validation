#include "../read_structure.hxx"

void read(std::string_view input = "structure.feature_flag.root",
          std::string_view output = "structure.feature_flag.json") {
  read_structure(input, output);
}

#include <ROOT/RNTupleReader.hxx>
#include <ROOT/RVersion.hxx>

#include <iostream>
#include <filesystem>

void read(std::string_view input = "structure.attributes.root",
          std::string_view output = "structure.attributes.json") {
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 40, 0)
  std::cout << "Skipped structure/attributes/read.C - ROOT version too old."
            << std::endl;
#else
  // skipping test if .root file file doesn't exist
  if (!std::filesystem::exists(input)) {
    std::cout << "Skipped structure/attributes/read.C - file ('" << input << "') not found" << std::endl;
    return;
  }
  // similar logic as read_structure.hxx
  std::unique_ptr<ROOT::RNTupleReader> reader = ROOT::RNTupleReader::Open("ntpl", input);

  std::ofstream os(std::string{output});
  os << "[\n";
  bool first = true;

  auto attrSet = reader->OpenAttributeSet("Attributes");
  auto attr = attrSet->GetModel().GetDefaultEntry().GetPtr<std::int32_t>("attr");
  
  for (auto attrIdx : attrSet->GetAttributes()) {
    auto range = attrSet->LoadEntry(attrIdx);
    if (first) {
      first = false;
    } else {
      os << ",\n";
    }
    os << "  {\n";
    os << "    \"Value\": " << *attr << ",\n";
    os << "    \"Range\": [" << *range.GetFirst() << ", " << *range.GetLast() << "]\n";
    os << "  }";
  }

  os << "\n]\n";
#endif
}

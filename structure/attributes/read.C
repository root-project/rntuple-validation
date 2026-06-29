#include <ROOT/RNTupleReader.hxx>
#include <iostream>
#include <ROOT/RVersion.hxx>
using namespace std;

void read(std::string_view input = "structure.attributes.root",
          std::string_view output = "structure.attributes.json") {
  #if ROOT_VERSION_CODE < ROOT_VERSION(6, 40, 0)
    cout << "Skipped structure/attributes/read.C - ROOT version too old." << endl;
    return void();
  #else
    // same logic as is read_structure.hxx, with additional special part for attributes
    std::unique_ptr<ROOT::RNTupleReader> reader;
    try {
      reader = ROOT::RNTupleReader::Open("ntpl", input);
    } catch (const ROOT::RException &e) {
      std::ofstream os(std::string{output});
      std::string msgWithoutStacktrace;
      std::getline(std::istringstream(e.what()), msgWithoutStacktrace);
      os << "{\n";
      os << "  \"error\": \"" << msgWithoutStacktrace << "\"\n";
      os << "}\n";
      return;
    } catch (const runtime_error &e) {
      cout << "Skipped structure/attributes/read.C - " << e.what() << endl;
      return void();
    }

    std::ofstream os(std::string{output});
    os << "[\n";
    auto Int32 =
        reader->GetModel().GetDefaultEntry().GetPtr<std::int32_t>("Int32");
    bool first = true;
    for (auto index : *reader) {
      reader->LoadEntry(index);

      if (first) {
        first = false;
      } else {
        os << ",\n";
      }
      os << "  {\n";
      os << "    \"Int32\": " << *Int32 << "\n";
      os << "  }";
      // Newline is intentionally missing, may need to print a comma before the
      // next entry.
    }

    auto attrSet = reader->OpenAttributeSet("Attributes");
    auto attrEntry = attrSet->CreateEntry();
    auto attrInt32 = attrEntry->GetPtr<std::int32_t>("attrInt32");
    
    for (auto idx : reader->GetEntryRange()) {
      for (auto attrIdx : attrSet->GetAttributes()) {
        attrSet->LoadEntry(attrIdx, *attrEntry);

        os << ",\n";
        os << "  {\n";
        os << "    \"attrInt32\": " << *attrInt32 << "\n";
        os << "  }";
      }
    }

    if (!first) {
      os << "\n";
    }

    os << "]\n";
  #endif
}

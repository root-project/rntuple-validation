#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <TSystem.h>

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "UserClass.hxx"

static void PrintUserMembers(const User &value, std::ostream &os,
                             std::string_view indent) {
  os << indent << "\":_0\": {\n";
  os << indent << "  \"fInt\": " << value.fInt << "\n";
  os << indent << "},\n";
  os << indent << "\"fString\": \"" << value.fString << "\",\n";

  os << indent << "\"fVariant\": ";
  if (value.fVariant.index() == 0) {
    os << std::get<std::int32_t>(value.fVariant);
  } else if (value.fVariant.index() == 1) {
    os << "\"" << std::get<std::string>(value.fVariant) << "\"";
  }
  os << ",\n";

  os << indent << "\"fVector\": [";
  bool first = true;
  for (auto element : value.fVector) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n" << indent << "  " << element;
  }
  if (!value.fVector.empty()) {
    os << "\n" << indent;
  }
  os << "],\n";

  os << indent << "\"fNested\": {\n";
  os << indent << "  \"fInt\": " << value.fNested.fInt << "\n";
  os << indent << "}\n";
}

static void PrintUserValue(const REntry &entry, std::string_view name,
                           std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<User>(name);
  os << "    \"" << name << "\": {\n";
  PrintUserMembers(value, os, "      ");
  os << "    }";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

static void PrintUserVector(const REntry &entry, std::string_view name,
                            std::ostream &os, bool last = false) {
  auto &vector = *entry.GetPtr<std::vector<User>>(name);
  os << "    \"" << name << "\": [";
  bool first = true;
  for (const auto &element : vector) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n      {\n";
    PrintUserMembers(element, os, "        ");
    os << "      }";
  }
  if (!vector.empty()) {
    os << "\n    ";
  }
  os << "]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.user.class.root",
          std::string_view output = "types.user.class.json") {
  if (gSystem->Load("libUserClass") == -1)
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");

  std::ofstream os(std::string{output});
  os << "[\n";

  auto reader = RNTupleReader::Open("ntpl", input);
  auto &entry = reader->GetModel().GetDefaultEntry();
  bool first = true;
  for (auto index : *reader) {
    reader->LoadEntry(index);

    if (first) {
      first = false;
    } else {
      os << ",\n";
    }
    os << "  {\n";

    PrintUserValue(entry, "f", os);
    PrintUserVector(entry, "Vector", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

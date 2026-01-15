#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>
#include <ROOT/RVec.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>

using Vector = ROOT::RVec<ROOT::RVec<std::int32_t>>;

static void PrintNestedVectorValue(const REntry &entry, std::string_view name,
                                   std::ostream &os, bool last = false) {
  Vector &value = *entry.GetPtr<Vector>(name);
  os << "    \"" << name << "\": [";
  bool outerFirst = true;
  for (auto inner : value) {
    if (outerFirst) {
      outerFirst = false;
    } else {
      os << ",";
    }
    os << "\n      [";
    bool innerFirst = true;
    for (auto element : inner) {
      if (innerFirst) {
        innerFirst = false;
      } else {
        os << ",";
      }
      os << "\n        " << element;
    }
    if (!inner.empty()) {
      os << "\n      ";
    }
    os << "]";
  }
  if (!value.empty()) {
    os << "\n    ";
  }
  os << "]";
  if (!last) {
    os << ",";
  }
  os << "\n";
}

void read(std::string_view input = "types.RVec.nested.root",
          std::string_view output = "types.RVec.nested.json") {
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

    PrintNestedVectorValue(entry, "Index32", os);
    PrintNestedVectorValue(entry, "Index64", os);
    PrintNestedVectorValue(entry, "SplitIndex32", os);
    PrintNestedVectorValue(entry, "SplitIndex64", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

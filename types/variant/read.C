#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using VectorInt32 = std::vector<std::int32_t>;
using Variant = std::variant<std::int32_t, std::string, VectorInt32>;
using Vector = std::vector<std::variant<std::int32_t, std::string>>;

static void PrintVariantValue(const REntry &entry, std::string_view name,
                              std::ostream &os, bool last = false) {
  Variant &value = *entry.GetPtr<Variant>(name);
  os << "    \"" << name << "\": ";
  if (value.index() == 0) {
    os << std::get<std::int32_t>(value);
  } else if (value.index() == 1) {
    os << "\"" << std::get<std::string>(value) << "\"";
  } else if (value.index() == 2) {
    VectorInt32 &vectorValue = std::get<VectorInt32>(value);
    os << "[";
    bool first = true;
    for (auto element : vectorValue) {
      if (first) {
        first = false;
      } else {
        os << ",";
      }
      os << "\n      " << element;
    }
    if (!vectorValue.empty()) {
      os << "\n    ";
    }
    os << "]";
  }

  if (!last) {
    os << ",";
  }
  os << "\n";
}

static void PrintVectorValue(const REntry &entry, std::string_view name,
                             std::ostream &os, bool last = false) {
  Vector &value = *entry.GetPtr<Vector>(name);
  os << "    \"" << name << "\": [";
  bool first = true;
  for (auto &element : value) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n      ";
    if (element.index() == 0) {
      os << std::get<std::int32_t>(element);
    } else if (element.index() == 1) {
      os << "\"" << std::get<std::string>(element) << "\"";
    }
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

void read(std::string_view input = "types.variant.root",
          std::string_view output = "types.variant.json") {
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

    PrintVariantValue(entry, "f", os);
    PrintVectorValue(entry, "Vector", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

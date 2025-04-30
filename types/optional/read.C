#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::Experimental::REntry;
using ROOT::Experimental::RNTupleReader;

#include <cstdint>
#include <fstream>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using VariantTy = std::variant<std::int32_t, std::string>;
using VectorInt32Ty = std::vector<std::int32_t>;

template <typename T> static void PrintValue(const T &value, std::ostream &os);

template <> void PrintValue(const std::int32_t &value, std::ostream &os) {
  os << value;
}

template <> void PrintValue(const std::string &value, std::ostream &os) {
  os << "\"" << value << "\"";
}

template <> void PrintValue(const VariantTy &value, std::ostream &os) {
  if (value.index() == 0) {
    PrintValue(std::get<std::int32_t>(value), os);
  } else if (value.index() == 1) {
    PrintValue(std::get<std::string>(value), os);
  }
}

template <> void PrintValue(const VectorInt32Ty &value, std::ostream &os) {
  os << "[";
  bool first = true;
  for (auto element : value) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n      " << element;
  }
  if (!value.empty()) {
    os << "\n    ";
  }
  os << "]";
}

template <typename T>
static void PrintOptionalValue(const REntry &entry, std::string_view name,
                               std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<std::optional<T>>(name);
  os << "    \"" << name << "\": ";
  if (!value.has_value()) {
    os << "null";
  } else {
    PrintValue(*value, os);
  }
  if (!last) {
    os << ",";
  }
  os << "\n";
}

static void PrintVectorOptValue(const REntry &entry, std::string_view name,
                                std::ostream &os, bool last = false) {
  auto &value = *entry.GetPtr<std::vector<std::optional<std::int32_t>>>(name);
  os << "    \"" << name << "\": [";
  bool first = true;
  for (auto &&element : value) {
    if (first) {
      first = false;
    } else {
      os << ",";
    }
    os << "\n      ";
    if (!element.has_value()) {
      os << "null";
    } else {
      os << *element;
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

void read(std::string_view input = "types.optional.root",
          std::string_view output = "types.optional.json") {
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

    PrintOptionalValue<std::int32_t>(entry, "Index32", os);
    PrintOptionalValue<std::int32_t>(entry, "Index64", os);
    PrintOptionalValue<std::int32_t>(entry, "SplitIndex32", os);
    PrintOptionalValue<std::int32_t>(entry, "SplitIndex64", os);
    PrintOptionalValue<std::string>(entry, "String", os);
    PrintOptionalValue<VariantTy>(entry, "Variant", os);
    PrintOptionalValue<VectorInt32Ty>(entry, "VectorInt32", os);
    PrintVectorOptValue(entry, "VectorOpt", os, /*last=*/true);

    os << "  }";
    // Newline is intentionally missing, may need to print a comma before the
    // next entry.
  }
  os << "\n";
  os << "]\n";
}

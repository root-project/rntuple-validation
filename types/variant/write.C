#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriter;

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using Vector = std::vector<std::int32_t>;
using Variant = std::variant<std::int32_t, std::string, Vector>;

void write(std::string_view filename = "types.variant.root") {
  auto model = RNTupleModel::Create();

  auto value = model->MakeField<Variant>("f");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: std::int32_t
  *value = 1;
  writer->Fill();

  // Second entry: std::string
  *value = "abc";
  writer->Fill();

  // Third entry: std::vector<std::int32_t>
  *value = Vector{1, 2, 3};
  writer->Fill();

  // Fourth entry: empty std::string
  *value = "";
  writer->Fill();

  // Fifth entry: empty std::vector
  *value = {};
  writer->Fill();
}

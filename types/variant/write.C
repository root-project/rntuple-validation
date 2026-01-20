#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using VectorInt32 = std::vector<std::int32_t>;
using Variant = std::variant<std::int32_t, std::string, VectorInt32>;
using Vector = std::vector<std::variant<std::int32_t, std::string>>;

void write(std::string_view filename = "types.variant.root") {
  auto model = RNTupleModel::Create();

  auto value = model->MakeField<Variant>("f");
  auto vector = model->MakeField<Vector>("Vector");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: std::int32_t
  *value = 1;
  *vector = {1};
  writer->Fill();

  // Second entry: std::string
  *value = "abc";
  *vector = {"abc"};
  writer->Fill();

  // Third entry: std::vector<std::int32_t>
  *value = VectorInt32{1, 2, 3};
  *vector = {1, "2", 3};
  writer->Fill();

  // Fourth entry: empty std::string
  *value = "";
  *vector = {""};
  writer->Fill();

  // Fifth entry: empty std::vector
  *value = VectorInt32{};
  *vector = {};
  writer->Fill();
}

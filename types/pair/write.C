#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

using Pair_Int32_String = std::pair<std::int32_t, std::string>;
using Variant = std::variant<std::int32_t, std::string>;
using VectorInt32 = std::vector<std::int32_t>;

void write(std::string_view filename = "types.pair.root") {
  auto model = RNTupleModel::Create();

  auto Int32_String = model->MakeField<Pair_Int32_String>("Int32_String");
  auto Variant_Vector =
      model->MakeField<std::pair<Variant, VectorInt32>>("Variant_Vector");
  auto Pair =
      model->MakeField<std::pair<Pair_Int32_String, std::int32_t>>("Pair");
  auto VectorPair =
      model->MakeField<std::vector<Pair_Int32_String>>("VectorPair");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: simple values
  *Int32_String = {1, "abc"};
  *Variant_Vector = {"def", {2, 3, 4}};
  *Pair = {{5, "ghi"}, 6};
  *VectorPair = {{7, "jkl"}, {8, "mno"}};
  writer->Fill();

  // Second entry: zero / empty values
  *Int32_String = {0, ""};
  *Variant_Vector = {0, {}};
  *Pair = {{0, ""}, 0};
  *VectorPair = {};
  writer->Fill();
}

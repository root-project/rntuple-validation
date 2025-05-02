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
#include <tuple>
#include <variant>
#include <vector>

using Tuple_Int32_String = std::tuple<std::int32_t, std::string>;
using VectorInt32 = std::vector<std::int32_t>;

void write(std::string_view filename = "types.tuple.root") {
  auto model = RNTupleModel::Create();

  auto Int32_String_Vector =
      model->MakeField<std::tuple<std::int32_t, std::string, VectorInt32>>(
          "Int32_String_Vector");
  auto Variant =
      model->MakeField<std::tuple<std::variant<std::int32_t, std::string>>>(
          "Variant");
  auto Tuple = model->MakeField<std::tuple<Tuple_Int32_String>>("Tuple");
  auto VectorTuple =
      model->MakeField<std::vector<Tuple_Int32_String>>("VectorTuple");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: simple values
  *Int32_String_Vector = {1, "abc", {2, 3, 4}};
  *Variant = {"def"};
  *Tuple = {{5, "ghi"}};
  *VectorTuple = {{6, "jkl"}, {7, "mno"}};
  writer->Fill();

  // Second entry: zero / empty values
  *Int32_String_Vector = {0, "", {}};
  *Variant = {0};
  *Tuple = {{0, ""}};
  *VectorTuple = {};
  writer->Fill();
}

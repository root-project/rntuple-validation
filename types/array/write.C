#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

using ArrayInt32 = std::array<std::int32_t, 2>;
using Variant = std::variant<std::int32_t, std::string>;
using VectorInt32 = std::vector<std::int32_t>;

void write(std::string_view filename = "types.array.root") {
  auto model = RNTupleModel::Create();

  auto Array_Int32 = model->MakeField<ArrayInt32>("Array_Int32");
  auto Array_Array = model->MakeField<std::array<ArrayInt32, 3>>("Array_Array");
  auto Array_String =
      model->MakeField<std::array<std::string, 2>>("Array_String");
  auto Array_Variant =
      model->MakeField<std::array<Variant, 2>>("Array_Variant");
  auto Array_Vector =
      model->MakeField<std::array<VectorInt32, 2>>("Array_Vector");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: simple values
  *Array_Int32 = {1, 2};
  *Array_Array = {ArrayInt32{3, 4}, ArrayInt32{5, 6}, ArrayInt32{7, 8}};
  *Array_String = {"ijk", "lmn"};
  *Array_Vector = {VectorInt32{15, 16, 17}, VectorInt32{17, 18}};
  *Array_Variant = {19, "uvw"};
  writer->Fill();

  // Second entry: zero / empty values
  *Array_Int32 = {0, 0};
  *Array_Array = {ArrayInt32{0, 0}, ArrayInt32{0, 0}, ArrayInt32{0, 0}};
  *Array_String = {"", ""};
  *Array_Vector = {VectorInt32{}, VectorInt32{}};
  *Array_Variant = {0, ""};
  writer->Fill();
}

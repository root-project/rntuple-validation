#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::EColumnType;
using ROOT::Experimental::RField;
using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <limits>
#include <memory>
#include <string_view>

static constexpr double pi = 3.14159265358979323846;

template <typename T>
static std::shared_ptr<T> MakeFieldReal32Quant(RNTupleModel &model,
                                               std::string_view name,
                                               int nBits, double min, double max) {
  assert(nBits >= 1 && nBits <= 32);
  assert(max > min);
  auto field = std::make_unique<RField<T>>(name);
  field->SetQuantized(min, max, nBits);
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<T>(name);
}

void write(std::string_view filename = "types.fundamental.real32quant.root") {
  auto model = RNTupleModel::Create();

  auto FloatReal32Quant1 =
      MakeFieldReal32Quant<float>(*model, "FloatReal32Quant1", 1, -1, 1);
  auto FloatReal32Quant8 =
      MakeFieldReal32Quant<float>(*model, "FloatReal32Quant8", 8, -1, 1);
  auto FloatReal32Quant32 =
      MakeFieldReal32Quant<float>(*model, "FloatReal32Quant32", 32, -(float)pi, (float)pi);
  auto DoubleReal32Quant1 =
      MakeFieldReal32Quant<double>(*model, "DoubleReal32Quant1", 1, -1, 1);
  auto DoubleReal32Quant20 =
      MakeFieldReal32Quant<double>(*model, "DoubleReal32Quant20", 20, -1, 1);
  auto DoubleReal32Quant32 =
      MakeFieldReal32Quant<double>(*model, "DoubleReal32Quant32", 32, -100, 25);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: ascending values
  *FloatReal32Quant1 = -1.0f;
  *FloatReal32Quant8 = -0.5f;
  *FloatReal32Quant32 = 0.25f;
  *DoubleReal32Quant1 = 0.5;
  *DoubleReal32Quant20 = 1.0;
  *DoubleReal32Quant32 = 2.0;
  writer->Fill();

  // Second entry: a value containing all 1s in the mantissa
  static constexpr float ValueAllOnes = 2.3509885e-38f;
  *FloatReal32Quant1 = ValueAllOnes;
  *FloatReal32Quant8 = ValueAllOnes;
  *FloatReal32Quant32 = ValueAllOnes;
  *DoubleReal32Quant1 = ValueAllOnes;
  *DoubleReal32Quant20 = ValueAllOnes;
  *DoubleReal32Quant32 = ValueAllOnes;
  writer->Fill();

  // Third entry: middle values of the value range
  *FloatReal32Quant1 = 0;
  *FloatReal32Quant8 = 0;
  *FloatReal32Quant32 = 0;
  *DoubleReal32Quant1 = 0;
  *DoubleReal32Quant20 = 0;
  *DoubleReal32Quant32 = -37.5;
  writer->Fill();

  // Fourth entry: min values
  *FloatReal32Quant1 = -1;
  *FloatReal32Quant8 = -1;
  *FloatReal32Quant32 = -pi;
  *DoubleReal32Quant1 = -1;
  *DoubleReal32Quant20 = -1;
  *DoubleReal32Quant32 = -100;
  writer->Fill();

  // Fifth entry: max values
  *FloatReal32Quant1 = 1;
  *FloatReal32Quant8 = 1;
  *FloatReal32Quant32 = pi;
  *DoubleReal32Quant1 = 1;
  *DoubleReal32Quant20 = 1;
  *DoubleReal32Quant32 = 25;
  writer->Fill();
}

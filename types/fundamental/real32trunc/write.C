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

template <typename T>
static std::shared_ptr<T> MakeFieldReal32Trunc(RNTupleModel &model,
                                               std::string_view name,
                                               int nBits) {
  assert(nBits >= 10 && nBits < 32);
  auto field = std::make_unique<RField<T>>(name);
  field->SetTruncated(nBits);
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<T>(name);
}

void write(std::string_view filename = "types.fundamental.real32trunc.root") {
  auto model = RNTupleModel::Create();

  auto FloatReal32Trunc10 =
      MakeFieldReal32Trunc<float>(*model, "FloatReal32Trunc10", 10);
  auto FloatReal32Trunc16 =
      MakeFieldReal32Trunc<float>(*model, "FloatReal32Trunc16", 16);
  auto FloatReal32Trunc31 =
      MakeFieldReal32Trunc<float>(*model, "FloatReal32Trunc31", 31);
  auto DoubleReal32Trunc10 =
      MakeFieldReal32Trunc<double>(*model, "DoubleReal32Trunc10", 10);
  auto DoubleReal32Trunc16 =
      MakeFieldReal32Trunc<double>(*model, "DoubleReal32Trunc16", 16);
  auto DoubleReal32Trunc31 =
      MakeFieldReal32Trunc<double>(*model, "DoubleReal32Trunc31", 31);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: ascending values
  *FloatReal32Trunc10 = 1.0f;
  *FloatReal32Trunc16 = 2.0f;
  *FloatReal32Trunc31 = 3.0f;
  *DoubleReal32Trunc10 = 4.0f;
  *DoubleReal32Trunc16 = 5.0f;
  *DoubleReal32Trunc31 = 6.0f;
  writer->Fill();

  // Second entry: a value containing all 1s in the mantissa
  static constexpr float Value32 = 3.9999997f;
  *FloatReal32Trunc10 = Value32;
  *FloatReal32Trunc16 = Value32;
  *FloatReal32Trunc31 = Value32;
  *DoubleReal32Trunc10 = Value32;
  *DoubleReal32Trunc16 = Value32;
  *DoubleReal32Trunc31 = Value32;
  writer->Fill();

  // Third entry: lowest values with N bits
  *FloatReal32Trunc10 = -0x1.8p127f;
  *FloatReal32Trunc16 = -0x1.FEp127f;
  *FloatReal32Trunc31 = std::numeric_limits<float>::lowest() + 0x1p-149f;
  *DoubleReal32Trunc10 = -0x1.8p127f;
  *DoubleReal32Trunc16 = -0x1.FEp127;
  *DoubleReal32Trunc31 = std::numeric_limits<float>::lowest() + 0x1p-149f;
  writer->Fill();

  // Fourth entry: min = smallest positive normal values
  *FloatReal32Trunc10 = std::numeric_limits<float>::min();
  *FloatReal32Trunc16 = std::numeric_limits<float>::min();
  *FloatReal32Trunc31 = std::numeric_limits<float>::min();
  *DoubleReal32Trunc10 = std::numeric_limits<float>::min();
  *DoubleReal32Trunc16 = std::numeric_limits<float>::min();
  *DoubleReal32Trunc31 = std::numeric_limits<float>::min();
  writer->Fill();

  // Fifth entry: smallest positive subnormal values with N bits
  // (e.g. FloatReal32Trunc10 has a value with all 0s except a 1 in the 10th bit)
  *FloatReal32Trunc10 = 0x1p-127f;
  *FloatReal32Trunc16 = 0x1p-133f;
  *FloatReal32Trunc31 = 0x1p-148f;
  *DoubleReal32Trunc10 = 0x1p-127f;
  *DoubleReal32Trunc16 = 0x1p-133f;
  *DoubleReal32Trunc31 = 0x1p-148f;
  writer->Fill();

  // Sixth entry: maximum values
  *FloatReal32Trunc10 = 0x1.8p127f;
  *FloatReal32Trunc16 = 0x1.FEp127f;
  *FloatReal32Trunc31 = std::numeric_limits<float>::max() - 0x1p-149f;
  *DoubleReal32Trunc10 = 0x1.8p127f;
  *DoubleReal32Trunc16 = 0x1.FEp127;
  *DoubleReal32Trunc31 = std::numeric_limits<float>::max() - 0x1p-149f;
  writer->Fill();
}

#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::ENTupleColumnType;
using ROOT::RNTupleWriteOptions;
using ROOT::Experimental::RField;
using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriter;

#include <limits>
#include <memory>
#include <string_view>

template <typename T>
static std::shared_ptr<T> MakeFundamentalField(RNTupleModel &model,
                                               std::string_view name,
                                               ENTupleColumnType type) {
  auto field = std::make_unique<RField<T>>(name);
  field->SetColumnRepresentatives({{type}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<T>(name);
}

void write(std::string_view filename = "types.fundamental.real.root") {
  auto model = RNTupleModel::Create();

  // Non-split float encoding
  auto FloatReal16 = MakeFundamentalField<float>(*model, "FloatReal16",
                                                 ENTupleColumnType::kReal16);
  auto FloatReal32 = MakeFundamentalField<float>(*model, "FloatReal32",
                                                 ENTupleColumnType::kReal32);
  auto DoubleReal16 = MakeFundamentalField<double>(*model, "DoubleReal16",
                                                   ENTupleColumnType::kReal16);
  auto DoubleReal32 = MakeFundamentalField<double>(*model, "DoubleReal32",
                                                   ENTupleColumnType::kReal32);
  auto DoubleReal64 = MakeFundamentalField<double>(*model, "DoubleReal64",
                                                   ENTupleColumnType::kReal64);

  // Split float encoding
  // NB there is no kSplitReal16
  auto FloatSplitReal32 = MakeFundamentalField<float>(
      *model, "FloatSplitReal32", ENTupleColumnType::kSplitReal32);
  auto DoubleSplitReal32 = MakeFundamentalField<double>(
      *model, "DoubleSplitReal32", ENTupleColumnType::kSplitReal32);
  auto DoubleSplitReal64 = MakeFundamentalField<double>(
      *model, "DoubleSplitReal64", ENTupleColumnType::kSplitReal64);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: ascending values
  *FloatReal16 = 1.0f;
  *FloatReal32 = 2.0f;
  *DoubleReal16 = 3.0;
  *DoubleReal32 = 4.0;
  *DoubleReal64 = 5.0;
  *FloatSplitReal32 = 6.0f;
  *DoubleSplitReal32 = 7.0;
  *DoubleSplitReal64 = 8.0;
  writer->Fill();

  // Second entry: values in each byte (to validate split encoding)
  // (enough digits to fill the fraction of 10, 23, 52 bits)
  static constexpr float Value16 = 0x1.234p1f;
  static constexpr float Value32 = 0x1.23456p1f;
  static constexpr double Value64 = 0x1.2345678901123p1;
  *FloatReal16 = Value16;
  *FloatReal32 = Value32;
  *DoubleReal16 = Value16;
  *DoubleReal32 = Value32;
  *DoubleReal64 = Value64;
  *FloatSplitReal32 = Value32;
  *DoubleSplitReal32 = Value32;
  *DoubleSplitReal64 = Value64;
  writer->Fill();

  // Third entry: lowest values
  static constexpr float Real16Lowest = -(0x1p16f - 0x1p5f);
  *FloatReal16 = Real16Lowest;
  *FloatReal32 = std::numeric_limits<float>::lowest();
  *DoubleReal16 = Real16Lowest;
  *DoubleReal32 = std::numeric_limits<float>::lowest();
  *DoubleReal64 = std::numeric_limits<double>::lowest();
  *FloatSplitReal32 = std::numeric_limits<float>::lowest();
  *DoubleSplitReal32 = std::numeric_limits<float>::lowest();
  *DoubleSplitReal64 = std::numeric_limits<double>::lowest();
  writer->Fill();

  // Fourth entry: min = smallest positive normal values
  static constexpr float Real16Min = 0x1p-14f;
  *FloatReal16 = Real16Min;
  *FloatReal32 = std::numeric_limits<float>::min();
  *DoubleReal16 = Real16Min;
  *DoubleReal32 = std::numeric_limits<float>::min();
  *DoubleReal64 = std::numeric_limits<double>::min();
  *FloatSplitReal32 = std::numeric_limits<float>::min();
  *DoubleSplitReal32 = std::numeric_limits<float>::min();
  *DoubleSplitReal64 = std::numeric_limits<double>::min();
  writer->Fill();

  // Fifth entry: denorm_min = smallest positive subnormal values
  static constexpr float Real16DenormMin = 0x1p-24f;
  *FloatReal16 = Real16DenormMin;
  *FloatReal32 = std::numeric_limits<float>::denorm_min();
  *DoubleReal16 = Real16DenormMin;
  *DoubleReal32 = std::numeric_limits<float>::denorm_min();
  *DoubleReal64 = std::numeric_limits<double>::denorm_min();
  *FloatSplitReal32 = std::numeric_limits<float>::denorm_min();
  *DoubleSplitReal32 = std::numeric_limits<float>::denorm_min();
  *DoubleSplitReal64 = std::numeric_limits<double>::denorm_min();
  writer->Fill();

  // Sixth entry: maximum values
  static constexpr float Real16Max = -Real16Lowest;
  *FloatReal16 = Real16Max;
  *FloatReal32 = std::numeric_limits<float>::max();
  *DoubleReal16 = Real16Max;
  *DoubleReal32 = std::numeric_limits<float>::max();
  *DoubleReal64 = std::numeric_limits<double>::max();
  *FloatSplitReal32 = std::numeric_limits<float>::max();
  *DoubleSplitReal32 = std::numeric_limits<float>::max();
  *DoubleSplitReal64 = std::numeric_limits<double>::max();
  writer->Fill();
}

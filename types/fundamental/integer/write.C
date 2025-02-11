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

#include <cstdint>
#include <limits>
#include <memory>
#include <string_view>

template <typename T>
static std::shared_ptr<T> MakeFundamentalField(RNTupleModel &model,
                                               std::string_view name,
                                               EColumnType type) {
  auto field = std::make_unique<RField<T>>(name);
  field->SetColumnRepresentatives({{type}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<T>(name);
}

void write(std::string_view filename = "types.fundamental.integer.root") {
  auto model = RNTupleModel::Create();

  auto Int8 =
      MakeFundamentalField<std::int8_t>(*model, "Int8", EColumnType::kInt8);
  auto UInt8 =
      MakeFundamentalField<std::uint8_t>(*model, "UInt8", EColumnType::kUInt8);

  // Non-split integer encoding
  auto Int16 =
      MakeFundamentalField<std::int16_t>(*model, "Int16", EColumnType::kInt16);
  auto UInt16 = MakeFundamentalField<std::uint16_t>(*model, "UInt16",
                                                    EColumnType::kUInt16);
  auto Int32 =
      MakeFundamentalField<std::int32_t>(*model, "Int32", EColumnType::kInt32);
  auto UInt32 = MakeFundamentalField<std::uint32_t>(*model, "UInt32",
                                                    EColumnType::kUInt32);
  auto Int64 =
      MakeFundamentalField<std::int64_t>(*model, "Int64", EColumnType::kInt64);
  auto UInt64 = MakeFundamentalField<std::uint64_t>(*model, "UInt64",
                                                    EColumnType::kUInt64);

  // Split integer encoding
  auto SplitInt16 = MakeFundamentalField<std::int16_t>(
      *model, "SplitInt16", EColumnType::kSplitInt16);
  auto SplitUInt16 = MakeFundamentalField<std::uint16_t>(
      *model, "SplitUInt16", EColumnType::kSplitUInt16);
  auto SplitInt32 = MakeFundamentalField<std::int32_t>(
      *model, "SplitInt32", EColumnType::kSplitInt32);
  auto SplitUInt32 = MakeFundamentalField<std::uint32_t>(
      *model, "SplitUInt32", EColumnType::kSplitUInt32);
  auto SplitInt64 = MakeFundamentalField<std::int64_t>(
      *model, "SplitInt64", EColumnType::kSplitInt64);
  auto SplitUInt64 = MakeFundamentalField<std::uint64_t>(
      *model, "SplitUInt64", EColumnType::kSplitUInt64);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: ascending values
  *Int8 = 1;
  *UInt8 = 2;
  *Int16 = 3;
  *UInt16 = 4;
  *Int32 = 5;
  *UInt32 = 6;
  *Int64 = 7;
  *UInt64 = 8;
  *SplitInt16 = 9;
  *SplitUInt16 = 10;
  *SplitInt32 = 11;
  *SplitUInt32 = 12;
  *SplitInt64 = 13;
  *SplitUInt64 = 14;
  writer->Fill();

  // Second entry: values in each byte (to validate split encoding)
  static constexpr std::uint8_t Value8 = 0x12;         // = 18
  static constexpr std::uint16_t Value16 = 0x1234;     // = 4660
  static constexpr std::uint32_t Value32 = 0x12345678; // = 305419896
  static constexpr std::uint64_t Value64 =
      0x123456780112358c; // = 1311768464885691788
  *Int8 = Value8;
  *UInt8 = Value8;
  *Int16 = Value16;
  *UInt16 = Value16;
  *Int32 = Value32;
  *UInt32 = Value32;
  *Int64 = Value64;
  *UInt64 = Value64;
  *SplitInt16 = Value16;
  *SplitUInt16 = Value16;
  *SplitInt32 = Value32;
  *SplitUInt32 = Value32;
  *SplitInt64 = Value64;
  *SplitUInt64 = Value64;
  writer->Fill();

  // Third entry: negative values for signed integer types (to validate zigzag
  // encoding)
  static constexpr std::int8_t NegativeValue8 = 0x92;         // = -110
  static constexpr std::int16_t NegativeValue16 = 0x9234;     // = -28108
  static constexpr std::int32_t NegativeValue32 = 0x92345678; // = -1842063752
  static constexpr std::int64_t NegativeValue64 =
      0x923456780112358c; // = -7911603571969084020
  *Int8 = NegativeValue8;
  *UInt8 = Value8;
  *Int16 = NegativeValue16;
  *UInt16 = Value16;
  *Int32 = NegativeValue32;
  *UInt32 = Value32;
  *Int64 = NegativeValue64;
  *UInt64 = Value64;
  *SplitInt16 = NegativeValue16;
  *SplitUInt16 = Value16;
  *SplitInt32 = NegativeValue32;
  *SplitUInt32 = Value32;
  *SplitInt64 = NegativeValue64;
  *SplitUInt64 = Value64;
  writer->Fill();

  // Fourth entry: minimum (lowest) values
  *Int8 = std::numeric_limits<std::int8_t>::lowest();
  *UInt8 = std::numeric_limits<std::uint8_t>::lowest();
  *Int16 = std::numeric_limits<std::int16_t>::lowest();
  *UInt16 = std::numeric_limits<std::uint16_t>::lowest();
  *Int32 = std::numeric_limits<std::int32_t>::lowest();
  *UInt32 = std::numeric_limits<std::uint32_t>::lowest();
  *Int64 = std::numeric_limits<std::int64_t>::lowest();
  *UInt64 = std::numeric_limits<std::uint64_t>::lowest();
  *SplitInt16 = std::numeric_limits<std::int16_t>::lowest();
  *SplitUInt16 = std::numeric_limits<std::uint16_t>::lowest();
  *SplitInt32 = std::numeric_limits<std::int32_t>::lowest();
  *SplitUInt32 = std::numeric_limits<std::uint32_t>::lowest();
  *SplitInt64 = std::numeric_limits<std::int64_t>::lowest();
  *SplitUInt64 = std::numeric_limits<std::uint64_t>::lowest();
  writer->Fill();

  // Fifth entry: maximum values
  *Int8 = std::numeric_limits<std::int8_t>::max();
  *UInt8 = std::numeric_limits<std::uint8_t>::max();
  *Int16 = std::numeric_limits<std::int16_t>::max();
  *UInt16 = std::numeric_limits<std::uint16_t>::max();
  *Int32 = std::numeric_limits<std::int32_t>::max();
  *UInt32 = std::numeric_limits<std::uint32_t>::max();
  *Int64 = std::numeric_limits<std::int64_t>::max();
  *UInt64 = std::numeric_limits<std::uint64_t>::max();
  *SplitInt16 = std::numeric_limits<std::int16_t>::max();
  *SplitUInt16 = std::numeric_limits<std::uint16_t>::max();
  *SplitInt32 = std::numeric_limits<std::int32_t>::max();
  *SplitUInt32 = std::numeric_limits<std::uint32_t>::max();
  *SplitInt64 = std::numeric_limits<std::int64_t>::max();
  *SplitUInt64 = std::numeric_limits<std::uint64_t>::max();
  writer->Fill();
}

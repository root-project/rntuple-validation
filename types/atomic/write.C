#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <atomic>
#include <cstddef> // for std::byte
#include <cstdint>
#include <memory>
#include <string_view>

template <typename T>
static std::shared_ptr<std::atomic<T>> MakeAtomicField(RNTupleModel &model,
                                                       std::string_view name) {
  return model.MakeField<std::atomic<T>>(name);
}

void write(std::string_view filename = "types.atomic.root") {
  auto model = RNTupleModel::Create();

  auto Bit = MakeAtomicField<bool>(*model, "Bit");
  auto Byte = MakeAtomicField<std::byte>(*model, "Byte");
  auto Char = MakeAtomicField<char>(*model, "Char");

  auto Int8 = MakeAtomicField<std::int8_t>(*model, "Int8");
  auto UInt8 = MakeAtomicField<std::uint8_t>(*model, "UInt8");
  auto Int16 = MakeAtomicField<std::int16_t>(*model, "Int16");
  auto UInt16 = MakeAtomicField<std::uint16_t>(*model, "UInt16");
  auto Int32 = MakeAtomicField<std::int32_t>(*model, "Int32");
  auto UInt32 = MakeAtomicField<std::uint32_t>(*model, "UInt32");
  auto Int64 = MakeAtomicField<std::int64_t>(*model, "Int64");
  auto UInt64 = MakeAtomicField<std::uint64_t>(*model, "UInt64");

  auto Real32 = MakeAtomicField<float>(*model, "Real32");
  auto Real64 = MakeAtomicField<double>(*model, "Real64");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: ascending values
  *Bit = 1;
  *Byte = std::byte{2};
  *Char = 3;
  *Int8 = 4;
  *UInt8 = 5;
  *Int16 = 6;
  *UInt16 = 7;
  *Int32 = 8;
  *UInt32 = 9;
  *Int64 = 10;
  *UInt64 = 11;
  *Real32 = 12.0f;
  *Real64 = 13.0;
  writer->Fill();

  // Second entry: zero values
  *Bit = 0;
  *Byte = std::byte{0};
  *Char = 0;
  *Int8 = 0;
  *UInt8 = 0;
  *Int16 = 0;
  *UInt16 = 0;
  *Int32 = 0;
  *UInt32 = 0;
  *Int64 = 0;
  *UInt64 = 0;
  *Real32 = 0;
  *Real64 = 0;
  writer->Fill();
}

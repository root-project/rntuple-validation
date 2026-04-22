#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

#include <cstddef> // for std::byte
#include <limits>
#include <memory>
#include <string_view>

template <typename T>
static std::shared_ptr<T> MakeFundamentalField(ROOT::RNTupleModel &model,
                                               std::string_view name,
                                               ROOT::ENTupleColumnType type) {
  auto field = std::make_unique<ROOT::RField<T>>(name);
  field->SetColumnRepresentatives({{type}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<T>(name);
}

void write(std::string_view filename = "types.fundamental.misc.root") {
  auto model = ROOT::RNTupleModel::Create();

  auto Bit =
      MakeFundamentalField<bool>(*model, "Bit", ROOT::ENTupleColumnType::kBit);
  auto Byte = MakeFundamentalField<std::byte>(*model, "Byte",
                                              ROOT::ENTupleColumnType::kByte);
  auto Char = MakeFundamentalField<char>(*model, "Char",
                                         ROOT::ENTupleColumnType::kChar);

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // First entry: ascending values
  *Bit = 1;
  *Byte = std::byte{2};
  *Char = 3;
  writer->Fill();

  // Second entry: low values
  *Bit = 0;
  *Byte = std::byte{0};
  *Char = 0;
  writer->Fill();

  // Third entry: high values
  *Bit = 1;
  *Byte = std::byte{255};
  *Char = 127; // a char can be signed or unsigned
  writer->Fill();
}

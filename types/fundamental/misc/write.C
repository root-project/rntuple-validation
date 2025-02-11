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

#include <cstddef> // for std::byte
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

void write(std::string_view filename = "types.fundamental.misc.root") {
  auto model = RNTupleModel::Create();

  auto Bit = MakeFundamentalField<bool>(*model, "Bit", EColumnType::kBit);
  auto Byte =
      MakeFundamentalField<std::byte>(*model, "Byte", EColumnType::kByte);
  auto Char = MakeFundamentalField<char>(*model, "Char", EColumnType::kChar);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

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

#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::ENTupleColumnType;
using ROOT::Experimental::RField;
using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <memory>
#include <string>
#include <string_view>

static std::shared_ptr<std::string>
MakeStringField(RNTupleModel &model, std::string_view name,
                ENTupleColumnType indexType) {
  auto field = std::make_unique<RField<std::string>>(name);
  field->SetColumnRepresentatives({{indexType, ENTupleColumnType::kChar}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<std::string>(name);
}

void write(std::string_view filename = "types.string.root") {
  auto model = RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 =
      MakeStringField(*model, "Index32", ENTupleColumnType::kIndex32);
  auto Index64 =
      MakeStringField(*model, "Index64", ENTupleColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 =
      MakeStringField(*model, "SplitIndex32", ENTupleColumnType::kSplitIndex32);
  auto SplitIndex64 =
      MakeStringField(*model, "SplitIndex64", ENTupleColumnType::kSplitIndex64);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: one character strings, with ascending values
  *Index32 = "a";
  *Index64 = "b";
  *SplitIndex32 = "c";
  *SplitIndex64 = "d";
  writer->Fill();

  // Second entry: empty strings
  *Index32 = "";
  *Index64 = "";
  *SplitIndex32 = "";
  *SplitIndex64 = "";
  writer->Fill();

  // Third entry: Unicode characters as UTF-8
  *Index32 = "🐣";
  *Index64 = "🐣";
  *SplitIndex32 = "🐣";
  *SplitIndex64 = "🐣";
  writer->Fill();

  // Fourth entry: increasing length of strings
  *Index32 = "a";
  *Index64 = "bc";
  *SplitIndex32 = "def";
  *SplitIndex64 = "ghij";
  writer->Fill();
}

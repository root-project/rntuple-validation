#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#if __has_include(<ROOT/RNTupleTypes.hxx>)
#include <ROOT/RNTupleTypes.hxx>
#else
#include <ROOT/RNTupleUtil.hxx>
#endif
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

#include <memory>
#include <string>
#include <string_view>

static std::shared_ptr<std::string>
MakeStringField(ROOT::RNTupleModel &model, std::string_view name,
                ROOT::ENTupleColumnType indexType) {
  auto field = std::make_unique<ROOT::RField<std::string>>(name);
  field->SetColumnRepresentatives(
      {{indexType, ROOT::ENTupleColumnType::kChar}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<std::string>(name);
}

void write(std::string_view filename = "types.string.root") {
  auto model = ROOT::RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 =
      MakeStringField(*model, "Index32", ROOT::ENTupleColumnType::kIndex32);
  auto Index64 =
      MakeStringField(*model, "Index64", ROOT::ENTupleColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 = MakeStringField(*model, "SplitIndex32",
                                      ROOT::ENTupleColumnType::kSplitIndex32);
  auto SplitIndex64 = MakeStringField(*model, "SplitIndex64",
                                      ROOT::ENTupleColumnType::kSplitIndex64);

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

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

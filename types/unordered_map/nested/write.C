#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#if __has_include(<ROOT/RNTupleTypes.hxx>)
#include <ROOT/RNTupleTypes.hxx>
#else
#include <ROOT/RNTupleUtil.hxx>
#endif
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

#include <TSystem.h>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string_view>
#include <unordered_map>

using UnorderedMap = std::unordered_map<std::string,
                               std::unordered_map<std::string, std::int32_t>>;

static std::shared_ptr<UnorderedMap>
MakeUnorderedMapField(ROOT::RNTupleModel &model, std::string_view name,
                      ROOT::ENTupleColumnType indexType) {
  auto field = std::make_unique<ROOT::RField<UnorderedMap>>(name);
  field->SetColumnRepresentatives({{indexType}});
  field->GetMutableSubfields()[0]
      ->GetMutableSubfields()[1]
      ->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<UnorderedMap>(name);
}

void write(std::string_view filename = "types.unordered_map.nested.root") {
  if (gSystem->Load("libNestedUnorderedMap") == -1)
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");

  auto model = ROOT::RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 = MakeUnorderedMapField(*model, "Index32",
                                       ROOT::ENTupleColumnType::kIndex32);
  auto Index64 = MakeUnorderedMapField(*model, "Index64",
                                       ROOT::ENTupleColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 = MakeUnorderedMapField(
      *model, "SplitIndex32", ROOT::ENTupleColumnType::kSplitIndex32);
  auto SplitIndex64 = MakeUnorderedMapField(
      *model, "SplitIndex64", ROOT::ENTupleColumnType::kSplitIndex64);

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // First entry: single-element maps, with ascending values
  *Index32 = {{"a", {{"aa", 1}}}};
  *Index64 = {{"b", {{"bb", 2}}}};
  *SplitIndex32 = {{"c", {{"cc", 3}}}};
  *SplitIndex64 = {{"d", {{"dd", 4}}}};
  writer->Fill();

  // Second entry: empty maps
  *Index32 = {};
  *Index64 = {};
  *SplitIndex32 = {};
  *SplitIndex64 = {};
  writer->Fill();

  // Third entry: increasing number of elements in the outer map
  *Index32 = {{"a", {{"aa", 1}}}};
  *Index64 = {{"b", {{"ba", 2}}}, {"c", {{"ca", 3}}}};
  *SplitIndex32 = {
      {"d", {{"da", 4}}}, {"e", {{"ea", 5}, {"eb", 6}}}, {"f", {}}};
  *SplitIndex64 = {{"g", {{"ga", 7}, {"gb", 8}}},
                   {"h", {}},
                   {"i", {{"ia", 9}}},
                   {"j", {{"ja", 10}, {"jb", 11}, {"jc", 12}}}};
  writer->Fill();
}

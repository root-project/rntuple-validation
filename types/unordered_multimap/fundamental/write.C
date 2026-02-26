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
#include <memory>
#include <string_view>
#include <unordered_map>

using UnorderedMultimap = std::unordered_multimap<std::string, std::int32_t>;

static std::shared_ptr<UnorderedMultimap> MakeMultimapField(RNTupleModel &model,
                                                  std::string_view name,
                                                  EColumnType indexType) {
  auto field = std::make_unique<RField<UnorderedMultimap>>(name);
  field->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<UnorderedMultimap>(name);
}

void write(std::string_view filename = "types.unordered_multimap.fundamental.root") {
  auto model = RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 = MakeMultimapField(*model, "Index32", EColumnType::kIndex32);
  auto Index64 = MakeMultimapField(*model, "Index64", EColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 =
      MakeMultimapField(*model, "SplitIndex32", EColumnType::kSplitIndex32);
  auto SplitIndex64 =
      MakeMultimapField(*model, "SplitIndex64", EColumnType::kSplitIndex64);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: single-element maps, with ascending values
  *Index32 = {{"a", 1}};
  *Index64 = {{"b", 2}};
  *SplitIndex32 = {{"c", 3}};
  *SplitIndex64 = {{"d", 4}};
  writer->Fill();

  // Second entry: empty maps
  *Index32 = {};
  *Index64 = {};
  *SplitIndex32 = {};
  *SplitIndex64 = {};
  writer->Fill();

  // Third entry: increasing number of elements in the map
  *Index32 = {{"a", 1}};
  *Index64 = {{"b", 2}, {"c", 3}};
  *SplitIndex32 = {{"d", 4}, {"e", 5}, {"f", 6}};
  *SplitIndex64 = {{"g", 7}, {"h", 8}, {"i", 9}, {"j", 10}};
  writer->Fill();
}

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
#include <unordered_set>

#include "NestedUnorderedMultiset.hxx"

using UnorderedMultiset =
    std::unordered_multiset<std::unordered_multiset<std::int32_t>>;

static std::shared_ptr<UnorderedMultiset>
MakeUnorderedMultisetField(ROOT::RNTupleModel &model, std::string_view name,
                           ROOT::ENTupleColumnType indexType) {
  auto field = std::make_unique<ROOT::RField<UnorderedMultiset>>(name);
  field->SetColumnRepresentatives({{indexType}});
  field->GetMutableSubfields()[0]->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<UnorderedMultiset>(name);
}

void write(std::string_view filename = "types.unordered_multiset.nested.root") {
  if (gSystem->Load("libNestedUnorderedMultiset") == -1)
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");

  auto model = ROOT::RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 = MakeUnorderedMultisetField(*model, "Index32",
                                            ROOT::ENTupleColumnType::kIndex32);
  auto Index64 = MakeUnorderedMultisetField(*model, "Index64",
                                            ROOT::ENTupleColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 = MakeUnorderedMultisetField(
      *model, "SplitIndex32", ROOT::ENTupleColumnType::kSplitIndex32);
  auto SplitIndex64 = MakeUnorderedMultisetField(
      *model, "SplitIndex64", ROOT::ENTupleColumnType::kSplitIndex64);

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // First entry: single-element sets, with ascending values
  *Index32 = {{1}};
  *Index64 = {{2}};
  *SplitIndex32 = {{3}};
  *SplitIndex64 = {{4}};
  writer->Fill();

  // Second entry: empty sets
  *Index32 = {};
  *Index64 = {};
  *SplitIndex32 = {};
  *SplitIndex64 = {};
  writer->Fill();

  // Third entry: increasing number of elements in the set
  *Index32 = {{1}};
  *Index64 = {{}, {2, 3}};
  *SplitIndex32 = {{}, {4}, {5, 6}};
  *SplitIndex64 = {{}, {7, 8, 9}, {10}, {11, 12}};
  writer->Fill();
}

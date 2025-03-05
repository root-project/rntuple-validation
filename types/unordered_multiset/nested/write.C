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
MakeUnorderedMultisetField(RNTupleModel &model, std::string_view name,
                           EColumnType indexType) {
  auto field = std::make_unique<RField<UnorderedMultiset>>(name);
  field->SetColumnRepresentatives({{indexType}});
  field->GetSubFields()[0]->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<UnorderedMultiset>(name);
}

void write(std::string_view filename = "types.unordered_multiset.nested.root") {
  if (!std::filesystem::exists("libNestedUnorderedMultiset.so")) {
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");
  }
  gSystem->Load("libNestedUnorderedMultiset");

  auto model = RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 =
      MakeUnorderedMultisetField(*model, "Index32", EColumnType::kIndex32);
  auto Index64 =
      MakeUnorderedMultisetField(*model, "Index64", EColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 = MakeUnorderedMultisetField(*model, "SplitIndex32",
                                                 EColumnType::kSplitIndex32);
  auto SplitIndex64 = MakeUnorderedMultisetField(*model, "SplitIndex64",
                                                 EColumnType::kSplitIndex64);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

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
  *SplitIndex32 = {{4}, {}, {5, 6}};
  *SplitIndex64 = {{}, {7, 8, 9}, {}, {10}};
  writer->Fill();

  // Fourth entry: duplicate elements in the set
  *Index32 = {{1}, {1}};
  *Index64 = {{2}, {2}};
  *SplitIndex32 = {{3}, {3}};
  *SplitIndex64 = {{4}, {4}};
  writer->Fill();

  // Fifth entry: unordered elements in the set
  *Index32 = {{3, 2}, {1}};
  *Index64 = {{}, {5, 4}};
  *SplitIndex32 = {{7}, {6}};
  *SplitIndex64 = {{9, 8}, {}};
  writer->Fill();

  // Sixth entry: duplicate and unordered elements in the set
  *Index32 = {{2, 1}, {}, {2, 1}};
  *Index64 = {{4, 4}, {3, 3}, {4, 4}};
  *SplitIndex32 = {{}, {6, 5}, {6, 5}};
  *SplitIndex64 = {{7, 7}, {7, 7}, {8, 8}};
  writer->Fill();
}

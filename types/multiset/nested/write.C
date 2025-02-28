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
#include <set>
#include <string_view>

using Multiset = std::multiset<std::multiset<std::int32_t>>;

static std::shared_ptr<Multiset> MakeMultisetField(RNTupleModel &model,
                                                   std::string_view name,
                                                   EColumnType indexType) {
  auto field = std::make_unique<RField<Multiset>>(name);
  field->SetColumnRepresentatives({{indexType}});
  field->GetSubFields()[0]->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<Multiset>(name);
}

void write(std::string_view filename = "types.multiset.nested.root") {
  if (!std::filesystem::exists("libNestedMultiset.so")) {
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");
  }

  gSystem->Load("libNestedMultiset");

  auto model = RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 = MakeMultisetField(*model, "Index32", EColumnType::kIndex32);
  auto Index64 = MakeMultisetField(*model, "Index64", EColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 =
      MakeMultisetField(*model, "SplitIndex32", EColumnType::kSplitIndex32);
  auto SplitIndex64 =
      MakeMultisetField(*model, "SplitIndex64", EColumnType::kSplitIndex64);

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
  *SplitIndex32 = {{}, {4}, {5, 6}};
  *SplitIndex64 = {{}, {7, 8, 9}, {10}, {11, 12}};
  writer->Fill();
}

#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::ENTupleColumnType;
using ROOT::RField;
using ROOT::RNTupleModel;
using ROOT::RNTupleWriteOptions;
using ROOT::RNTupleWriter;

#include <cstdint>
#include <memory>
#include <set>
#include <string_view>

using Multiset = std::multiset<std::int32_t>;

static std::shared_ptr<Multiset> MakeMultisetField(RNTupleModel &model,
                                                   std::string_view name,
                                                   ENTupleColumnType indexType) {
  auto field = std::make_unique<RField<Multiset>>(name);
  field->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<Multiset>(name);
}

void write(std::string_view filename = "types.multiset.fundamental.root") {
  auto model = RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 = MakeMultisetField(*model, "Index32", ENTupleColumnType::kIndex32);
  auto Index64 = MakeMultisetField(*model, "Index64", ENTupleColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 =
      MakeMultisetField(*model, "SplitIndex32", ENTupleColumnType::kSplitIndex32);
  auto SplitIndex64 =
      MakeMultisetField(*model, "SplitIndex64", ENTupleColumnType::kSplitIndex64);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: single-element sets, with ascending values
  *Index32 = {1};
  *Index64 = {2};
  *SplitIndex32 = {3};
  *SplitIndex64 = {4};
  writer->Fill();

  // Second entry: empty sets
  *Index32 = {};
  *Index64 = {};
  *SplitIndex32 = {};
  *SplitIndex64 = {};
  writer->Fill();

  // Third entry: increasing number of elements in the set
  *Index32 = {1};
  *Index64 = {2, 3};
  *SplitIndex32 = {4, 5, 6};
  *SplitIndex64 = {7, 8, 9, 10};
  writer->Fill();
}

#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <ROOT/RVec.hxx>

#include <cstdint>
#include <memory>
#include <string_view>

using Inner = ROOT::RVec<std::int32_t>;
using Vector = ROOT::RVec<Inner>;

static std::shared_ptr<Vector>
MakeVectorField(ROOT::RNTupleModel &model, std::string_view name,
                ROOT::ENTupleColumnType indexType) {
  auto field = std::make_unique<ROOT::RField<Vector>>(name);
  field->SetColumnRepresentatives({{indexType}});
  field->GetMutableSubfields()[0]->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<Vector>(name);
}

void write(std::string_view filename = "types.RVec.nested.root") {
  auto model = ROOT::RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 =
      MakeVectorField(*model, "Index32", ROOT::ENTupleColumnType::kIndex32);
  auto Index64 =
      MakeVectorField(*model, "Index64", ROOT::ENTupleColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 = MakeVectorField(*model, "SplitIndex32",
                                      ROOT::ENTupleColumnType::kSplitIndex32);
  auto SplitIndex64 = MakeVectorField(*model, "SplitIndex64",
                                      ROOT::ENTupleColumnType::kSplitIndex64);

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // First entry: single-element vectors, with ascending values
  *Index32 = {{1}};
  *Index64 = {{2}};
  *SplitIndex32 = {{3}};
  *SplitIndex64 = {{4}};
  writer->Fill();

  // Second entry: empty vectors
  *Index32 = {};
  *Index64 = {};
  *SplitIndex32 = {};
  *SplitIndex64 = {};
  writer->Fill();

  // Third entry: increasing number of elements in the outer vector
  *Index32 = {{1}};
  *Index64 = {{}, {2, 3}};
  *SplitIndex32 = {{4}, {}, {5, 6}};
  *SplitIndex64 = {{}, {7, 8, 9}, {}, {10}};
  writer->Fill();
}

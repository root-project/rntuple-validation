#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <ROOT/RVec.hxx>

using ROOT::Experimental::EColumnType;
using ROOT::Experimental::RField;
using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <cstdint>
#include <memory>
#include <string_view>

using Inner = ROOT::RVec<std::int32_t>;
using Vector = ROOT::RVec<Inner>;

static std::shared_ptr<Vector> MakeVectorField(RNTupleModel &model,
                                               std::string_view name,
                                               EColumnType indexType) {
  auto field = std::make_unique<RField<Vector>>(name);
  field->SetColumnRepresentatives({{indexType}});
  field->GetSubFields()[0]->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<Vector>(name);
}

void write(std::string_view filename = "types.RVec.nested.root") {
  auto model = RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 = MakeVectorField(*model, "Index32", EColumnType::kIndex32);
  auto Index64 = MakeVectorField(*model, "Index64", EColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 =
      MakeVectorField(*model, "SplitIndex32", EColumnType::kSplitIndex32);
  auto SplitIndex64 =
      MakeVectorField(*model, "SplitIndex64", EColumnType::kSplitIndex64);

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

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

  // Work around bug in 6.34 for destroying RVec's, later fixed by commit
  // https://github.com/root-project/root/commit/996cac359d for 6.36.
  Index32->clear();
  Index64->clear();
  SplitIndex32->clear();
  SplitIndex64->clear();
}

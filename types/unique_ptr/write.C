#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using PtrInt32Ty = std::unique_ptr<std::int32_t>;
using VariantTy = std::variant<std::int32_t, std::string>;
using VectorInt32Ty = std::vector<std::int32_t>;

static std::shared_ptr<PtrInt32Ty>
MakeIntField(ROOT::RNTupleModel &model, std::string_view name,
             ROOT::ENTupleColumnType indexType) {
  auto field = std::make_unique<ROOT::RField<PtrInt32Ty>>(name);
  field->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<PtrInt32Ty>(name);
}

void write(std::string_view filename = "types.unique_ptr.root") {
  auto model = ROOT::RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 =
      MakeIntField(*model, "Index32", ROOT::ENTupleColumnType::kIndex32);
  auto Index64 =
      MakeIntField(*model, "Index64", ROOT::ENTupleColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 = MakeIntField(*model, "SplitIndex32",
                                   ROOT::ENTupleColumnType::kSplitIndex32);
  auto SplitIndex64 = MakeIntField(*model, "SplitIndex64",
                                   ROOT::ENTupleColumnType::kSplitIndex64);

  auto String = model->MakeField<std::unique_ptr<std::string>>("String");
  auto Variant = model->MakeField<std::unique_ptr<VariantTy>>("Variant");
  auto VectorInt32 =
      model->MakeField<std::unique_ptr<VectorInt32Ty>>("VectorInt32");
  auto VectorPtr =
      model->MakeField<std::vector<std::unique_ptr<std::int32_t>>>("VectorPtr");

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // First entry: simple values
  *Index32 = std::make_unique<std::int32_t>(1);
  *Index64 = std::make_unique<std::int32_t>(2);
  *SplitIndex32 = std::make_unique<std::int32_t>(3);
  *SplitIndex64 = std::make_unique<std::int32_t>(4);
  *String = std::make_unique<std::string>("abc");
  *Variant = std::make_unique<VariantTy>("def");
  *VectorInt32 = std::make_unique<VectorInt32Ty>();
  **VectorInt32 = {1, 2, 3};
  VectorPtr->push_back(std::make_unique<std::int32_t>(4));
  VectorPtr->push_back(std::make_unique<std::int32_t>(5));
  VectorPtr->push_back(std::make_unique<std::int32_t>(6));
  writer->Fill();

  // Second entry: no value
  Index32->reset();
  Index64->reset();
  SplitIndex32->reset();
  SplitIndex64->reset();
  String->reset();
  Variant->reset();
  VectorInt32->reset();
  VectorPtr->clear();
  VectorPtr->push_back(nullptr);
  writer->Fill();

  // Third entry: zero / empty value
  *Index32 = std::make_unique<std::int32_t>(0);
  *Index64 = std::make_unique<std::int32_t>(0);
  *SplitIndex32 = std::make_unique<std::int32_t>(0);
  *SplitIndex64 = std::make_unique<std::int32_t>(0);
  *String = std::make_unique<std::string>("");
  *Variant = std::make_unique<VariantTy>(0);
  *VectorInt32 = std::make_unique<VectorInt32Ty>();
  VectorPtr->clear();
  writer->Fill();
}

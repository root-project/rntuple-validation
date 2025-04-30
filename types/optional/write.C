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
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using OptInt32Ty = std::optional<std::int32_t>;
using VariantTy = std::variant<std::int32_t, std::string>;
using VectorInt32Ty = std::vector<std::int32_t>;

static std::shared_ptr<OptInt32Ty> MakeIntField(RNTupleModel &model,
                                                std::string_view name,
                                                EColumnType indexType) {
  auto field = std::make_unique<RField<OptInt32Ty>>(name);
  field->SetColumnRepresentatives({{indexType}});
  model.AddField(std::move(field));
  return model.GetDefaultEntry().GetPtr<OptInt32Ty>(name);
}

void write(std::string_view filename = "types.optional.root") {
  auto model = RNTupleModel::Create();

  // Non-split index encoding
  auto Index32 = MakeIntField(*model, "Index32", EColumnType::kIndex32);
  auto Index64 = MakeIntField(*model, "Index64", EColumnType::kIndex64);

  // Split index encoding
  auto SplitIndex32 =
      MakeIntField(*model, "SplitIndex32", EColumnType::kSplitIndex32);
  auto SplitIndex64 =
      MakeIntField(*model, "SplitIndex64", EColumnType::kSplitIndex64);

  auto String = model->MakeField<std::optional<std::string>>("String");
  auto Variant = model->MakeField<std::optional<VariantTy>>("Variant");
  auto VectorInt32 =
      model->MakeField<std::optional<VectorInt32Ty>>("VectorInt32");
  auto VectorOpt =
      model->MakeField<std::vector<std::optional<std::int32_t>>>("VectorOpt");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: simple values
  *Index32 = 1;
  *Index64 = 2;
  *SplitIndex32 = 3;
  *SplitIndex64 = 4;
  *String = "abc";
  *Variant = "def";
  *VectorInt32 = {1, 2, 3};
  *VectorOpt = {4, 5, 6};
  writer->Fill();

  // Second entry: no value
  Index32->reset();
  Index64->reset();
  SplitIndex32->reset();
  SplitIndex64->reset();
  String->reset();
  Variant->reset();
  VectorInt32->reset();
  *VectorOpt = {{}};
  writer->Fill();

  // Third entry: zero / empty value
  *Index32 = 0;
  *Index64 = 0;
  *SplitIndex32 = 0;
  *SplitIndex64 = 0;
  *String = "";
  *Variant = 0;
  *VectorInt32 = VectorInt32Ty{};
  *VectorOpt = {};
  writer->Fill();
}

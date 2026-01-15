#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RField;
using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

template <typename T>
static void AddProjectedField(RNTupleModel &model, std::string_view name,
                              std::string_view source) {
  auto field = std::make_unique<RField<T>>(name);
  model.AddProjectedField(std::move(field), [&source](const std::string &) {
    return std::string{source};
  });
}

void write(std::string_view filename = "projections.leaf.root") {
  auto model = RNTupleModel::Create();

  auto Pair = model->MakeField<std::pair<std::int32_t, float>>("Pair");
  AddProjectedField<std::int32_t>(*model, "Int32", "Pair._0");
  AddProjectedField<float>(*model, "Float", "Pair._1");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: ascending values
  *Pair = {1, 2.0};
  writer->Fill();
}

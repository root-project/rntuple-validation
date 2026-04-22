#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <ROOT/RVec.hxx>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

template <typename T>
static void AddProjectedField(ROOT::RNTupleModel &model, std::string_view name,
                              std::string_view source) {
  auto field = std::make_unique<ROOT::RField<std::vector<T>>>(name);
  model.AddProjectedField(std::move(field),
                          [&name, &source](const std::string &fieldName) {
                            if (fieldName == name) {
                              return std::string("VectorPair");
                            } else {
                              return "VectorPair._0." + std::string(source);
                            }
                          });
}

void write(std::string_view filename = "projections.nested.root") {
  auto model = ROOT::RNTupleModel::Create();

  auto VectorPair =
      model->MakeField<std::vector<std::pair<std::int32_t, float>>>(
          "VectorPair");
  AddProjectedField<std::int32_t>(*model, "VectorInt", "_0");
  AddProjectedField<float>(*model, "VectorFloat", "_1");

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // First entry: ascending values
  VectorPair->emplace_back(1, 2.0);
  VectorPair->emplace_back(3, 4.0);
  writer->Fill();

  // Second entry: empty collection
  VectorPair->clear();
  writer->Fill();
}

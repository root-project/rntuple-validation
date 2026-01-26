#include <ROOT/RField.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <ROOT/RVec.hxx>

using ROOT::Experimental::RField;
using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

void write(std::string_view filename = "projections.collection.root") {
  auto model = RNTupleModel::Create();

  auto Vector = model->MakeField<std::vector<std::int32_t>>("Vector");
  auto Projected =
      std::make_unique<RField<ROOT::RVec<std::int32_t>>>("Projected");
  model->AddProjectedField(std::move(Projected),
                           [](const std::string &fieldName) {
                             if (fieldName == "Projected") {
                               return "Vector";
                             } else {
                               return "Vector._0";
                             }
                           });

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: ascending values
  *Vector = {1, 2};
  writer->Fill();

  // Second entry: empty collection
  Vector->clear();
  writer->Fill();
}

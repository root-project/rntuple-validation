#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

#include <cstdint>
#include <string_view>

void write(std::string_view filename = "structure.empty.root") {
  auto model = ROOT::RNTupleModel::Create();

  auto Int32 = model->MakeField<std::int32_t>("Int32");

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // Destruct the writer / commit the dataset without filling an entry.
  writer.reset();
}

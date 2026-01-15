#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <cstdint>
#include <string_view>

void write(std::string_view filename = "structure.empty.root") {
  auto model = RNTupleModel::Create();

  auto Int32 = model->MakeField<std::int32_t>("Int32");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // Destruct the writer / commit the dataset without filling an entry.
  writer.reset();
}

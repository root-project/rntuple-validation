#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleUtil.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::EColumnType;
using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <cstdint>
#include <memory>
#include <string_view>

void write(std::string_view filename = "compression.block.short.root") {
  auto model = RNTupleModel::Create();

  auto Int64 = model->MakeField<std::int64_t>("Int64");
  model->GetMutableField("Int64").SetColumnRepresentatives(
      {{EColumnType::kSplitInt64}});

  RNTupleWriteOptions options;
  options.SetCompression(505);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // Write only 2 entries to make sure the compression block is small and
  // actually stored uncompressed.
  for (int i = 0; i < 2; i++) {
    *Int64 = i;
    writer->Fill();
  }
}

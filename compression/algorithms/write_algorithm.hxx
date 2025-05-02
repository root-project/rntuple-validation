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

void write_algorithm(std::string_view filename, std::uint32_t compression) {
  auto model = RNTupleModel::Create();

  auto Int64 = model->MakeField<std::int64_t>("Int64");
  model->GetMutableField("Int64").SetColumnRepresentatives(
      {{EColumnType::kSplitInt64}});

  RNTupleWriteOptions options;
  options.SetCompression(compression);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // Write 32 entries to make sure the compression block is not too small.
  for (int i = 0; i < 32; i++) {
    *Int64 = i;
    writer->Fill();
  }
}

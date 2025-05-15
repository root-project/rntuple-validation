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

void write(std::string_view filename = "compression.block.big.root") {
  auto model = RNTupleModel::Create();

  auto Int64 = model->MakeField<std::int64_t>("Int64");
  model->GetMutableField("Int64").SetColumnRepresentatives(
      {{EColumnType::kSplitInt64}});

  RNTupleWriteOptions options;
  // Crank up the zstd compression level to reduce the output file size by
  // approximately a factor 6 (from 76K with 505 to 12K).
  options.SetCompression(509);
  // Increase the maximum unzipped page size to make it bigger than the maximum
  // size of a compression block, which is 16 MiB.
  options.SetMaxUnzippedPageSize(128 * 1024 * 1024);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // Write 40 MiB of entries that will be split into three compression blocks.
  static constexpr int Entries = 40 * 1024 * 1024 / sizeof(std::int64_t);
  for (int i = 0; i < Entries; i++) {
    *Int64 = i;
    writer->Fill();
  }
}

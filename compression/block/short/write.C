#include <ROOT/RNTupleModel.hxx>
#if __has_include(<ROOT/RNTupleTypes.hxx>)
#include <ROOT/RNTupleTypes.hxx>
#else
#include <ROOT/RNTupleUtil.hxx>
#endif
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

#include <cstdint>
#include <memory>
#include <string_view>

void write(std::string_view filename = "compression.block.short.root") {
  auto model = ROOT::RNTupleModel::Create();

  auto Int64 = model->MakeField<std::int64_t>("Int64");
  model->GetMutableField("Int64").SetColumnRepresentatives(
      {{ROOT::ENTupleColumnType::kSplitInt64}});

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(505);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // Write only 2 entries to make sure the compression block is small and
  // actually stored uncompressed.
  for (int i = 0; i < 2; i++) {
    *Int64 = i;
    writer->Fill();
  }
}

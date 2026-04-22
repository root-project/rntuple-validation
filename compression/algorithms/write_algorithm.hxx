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

void write_algorithm(std::string_view filename, std::uint32_t compression) {
  auto model = ROOT::RNTupleModel::Create();

  auto Int64 = model->MakeField<std::int64_t>("Int64");
  model->GetMutableField("Int64").SetColumnRepresentatives(
      {{ROOT::ENTupleColumnType::kSplitInt64}});

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(compression);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  // Write 32 entries to make sure the compression block is not too small.
  for (int i = 0; i < 32; i++) {
    *Int64 = i;
    writer->Fill();
  }
}

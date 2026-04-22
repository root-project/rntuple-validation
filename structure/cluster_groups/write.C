#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

#include <cstdint>
#include <string_view>

void write(std::string_view filename = "structure.cluster_groups.root") {
  auto model = ROOT::RNTupleModel::Create();

  auto Int32 = model->MakeField<std::int32_t>("Int32");

  ROOT::RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl",
                                              filename, options);

  *Int32 = 1;
  writer->Fill();
  writer->CommitCluster();

  *Int32 = 2;
  writer->Fill();
  writer->CommitCluster(/*commitClusterGroup=*/true);

  *Int32 = 3;
  writer->Fill();
  writer->CommitCluster();

  *Int32 = 4;
  writer->Fill();
  writer->CommitCluster(/*commitClusterGroup=*/true);
}

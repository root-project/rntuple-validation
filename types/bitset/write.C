#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <bitset>
#include <memory>
#include <string_view>

void write(std::string_view filename = "types.bitset.root") {
  auto model = RNTupleModel::Create();

  auto Bitset1 = model->MakeField<std::bitset<1>>("Bitset1");
  auto Bitset64 = model->MakeField<std::bitset<64>>("Bitset64");
  auto Bitset65 = model->MakeField<std::bitset<65>>("Bitset65");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: ascending bits set
  Bitset1->set(0);
  Bitset64->set(1);
  Bitset65->set(2);
  writer->Fill();

  // Second entry: all bits set
  Bitset1->set();
  Bitset64->set();
  Bitset65->set();
  writer->Fill();

  // Third entry: no bits set
  Bitset1->reset();
  Bitset64->reset();
  Bitset65->reset();
  writer->Fill();
}

#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <TSystem.h>

#include <string_view>

#include "UserEnum.hxx"

void write(std::string_view filename = "types.user.enum.root") {
  if (gSystem->Load("libUserEnum") == -1)
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");

  auto model = RNTupleModel::Create();

  // TODO: enums with bool underlying type are possible since ROOT 6.38
  auto EInt8 = model->MakeField<EnumInt8>("EnumInt8");
  auto EUInt8 = model->MakeField<EnumUInt8>("EnumUInt8");
  auto EInt16 = model->MakeField<EnumInt16>("EnumInt16");
  auto EUInt16 = model->MakeField<EnumUInt16>("EnumUInt16");
  auto EInt32 = model->MakeField<EnumInt32>("EnumInt32");
  auto EUInt32 = model->MakeField<EnumUInt32>("EnumUInt32");
  auto EInt64 = model->MakeField<EnumInt64>("EnumInt64");
  auto EUInt64 = model->MakeField<EnumUInt64>("EnumUInt64");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: simple values
  *EInt8 = EnumInt8::Simple;     // = 1;
  *EUInt8 = EnumUInt8::Simple;   // = 2;
  *EInt16 = EnumInt16::Simple;   // = 3;
  *EUInt16 = EnumUInt16::Simple; // = 4;
  *EInt32 = EnumInt32::Simple;   // = 5;
  *EUInt32 = EnumUInt32::Simple; // = 6;
  *EInt64 = EnumInt64::Simple;   // = 7;
  *EUInt64 = EnumUInt64::Simple; // = 8;
  writer->Fill();

  // Second entry: zero values
  *EInt8 = EnumInt8::Zero;
  *EUInt8 = EnumUInt8::Zero;
  *EInt16 = EnumInt16::Zero;
  *EUInt16 = EnumUInt16::Zero;
  *EInt32 = EnumInt32::Zero;
  *EUInt32 = EnumUInt32::Zero;
  *EInt64 = EnumInt64::Zero;
  *EUInt64 = EnumUInt64::Zero;
  writer->Fill();
}

#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::Experimental::RNTupleModel;
using ROOT::Experimental::RNTupleWriteOptions;
using ROOT::Experimental::RNTupleWriter;

#include <TSystem.h>

#include <string_view>

#include "UserClass.hxx"

void write(std::string_view filename = "types.user.class.root") {
  if (gSystem->Load("libUserClass") == -1)
    throw std::runtime_error("could not find the required ROOT dictionaries, "
                             "please make sure to run `make` first");

  auto model = RNTupleModel::Create();

  auto value = model->MakeField<User>("f");
  auto vector = model->MakeField<std::vector<User>>("Vector");

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer =
      RNTupleWriter::Recreate(std::move(model), "ntpl", filename, options);

  // First entry: simple values
  value->fInt = 1;
  value->fString = "b";
  value->fVariant = 3;
  value->fVector = {4};
  value->fNested.fInt = 5;

  vector->resize(2);
  vector->at(0).fInt = 6;
  vector->at(0).fString = "g";
  vector->at(0).fVariant = "h";
  vector->at(0).fVector = {9, 10};
  vector->at(0).fNested.fInt = 11;

  vector->at(1).fInt = 12;
  vector->at(1).fString = "m";
  vector->at(1).fVariant = 14;
  vector->at(1).fVector = {15};
  vector->at(1).fNested.fInt = 16;

  writer->Fill();

  // Second entry: zero / empty values
  value->fInt = 0;
  value->fString = "";
  value->fVariant = 0;
  value->fVector.clear();
  value->fNested.fInt = 0;
  vector->clear();
  writer->Fill();
}

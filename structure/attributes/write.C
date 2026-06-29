#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <ROOT/RVersion.hxx>

#include <TFile.h>
#include <cstdint>
#include <string_view>

void write(std::string_view filename = "structure.attributes.root") {
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 40, 0)
  std::cout << "Skipped structure/attributes/write.C - ROOT version too old."
            << std::endl;
#else
  auto model = ROOT::RNTupleModel::Create();
  auto Int32 = model->MakeField<std::int32_t>("Int32");

  auto file = std::unique_ptr<TFile>(TFile::Open(std::string(filename).c_str(), "RECREATE"));
  auto writer = ROOT::RNTupleWriter::Append(std::move(model), "ntpl", *file);

  // definition of attribute set with attribute fields
  auto attrModel = ROOT::RNTupleModel::Create();
  auto attr = attrModel->MakeField<std::int32_t>("attr");
  auto attrSet = writer->CreateAttributeSet(std::move(attrModel), "Attributes");

  // first entry
  auto attrRange = attrSet->BeginRange();
  *Int32 = 1;
  writer->Fill();
  *attr = 1;
  attrSet->CommitRange(std::move(attrRange));

  // second and third entry
  attrRange = attrSet->BeginRange();
  *attr = 2;
  *Int32 = 2;
  writer->Fill();
  *Int32 = 3;
  writer->Fill();
  attrSet->CommitRange(std::move(attrRange));
#endif
}

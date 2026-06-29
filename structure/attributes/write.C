#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <TFile.h>
#include <ROOT/RVersion.hxx>

#include <cstdint>
#include <string_view>

void write(std::string_view filename = "structure.attributes.root") {
  #if ROOT_VERSION_CODE < ROOT_VERSION(6, 40, 0)
    cout << "Skipped structure/attributes/write.C - ROOT version too old." << endl;
    return void();
  #else
    auto model = ROOT::RNTupleModel::Create();
    auto Int32 = model->MakeField<std::int32_t>("Int32");
    
    auto attrModel = ROOT::RNTupleModel::Create();
    auto attrInt32 = attrModel->MakeField<std::int32_t>("attrInt32");

    auto file = std::unique_ptr<TFile>(TFile::Open(std::string(filename).c_str(), "RECREATE"));
    auto writer = ROOT::RNTupleWriter::Append(std::move(model), "ntpl", *file);
    
    auto attrSet = writer->CreateAttributeSet(std::move(attrModel), "Attributes");
    auto attrRange = attrSet->BeginRange();
    *Int32 = 1;
    *attrInt32 = 3;
    writer->Fill();
    attrSet->CommitRange(std::move(attrRange));
  #endif
}

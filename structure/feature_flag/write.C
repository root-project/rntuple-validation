#include <ROOT/RField.hxx>
#include <ROOT/RMiniFile.hxx>
#include <ROOT/RNTupleDescriptor.hxx>
#include <ROOT/RNTupleSerialize.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>

#include <memory>
#include <string_view>
#include <utility>

using ROOT::RFieldZero;
using ROOT::RNTupleDescriptor;
using ROOT::RNTupleWriteOptions;
using ROOT::Internal::RFieldDescriptorBuilder;
using ROOT::Internal::RNTupleDescriptorBuilder;
using ROOT::Internal::RNTupleFileWriter;
using ROOT::Internal::RNTupleSerializer;

void write(std::string_view filename = "structure.feature_flag.root") {
  // Note that we are writing a file with a so-far unused feature flag. This
  // cannot use the regular production API but we have to use the internal,
  // low-level classes to create the file.

  RNTupleDescriptorBuilder descBuilder;
  // The following line will be required as of ROOT v6.36
  // descBuilder.SetVersionForWriting();
  descBuilder.SetNTuple("ntpl", "");
  descBuilder.SetFeature(RNTupleDescriptor::kFeatureFlagTest);
  descBuilder.AddField(RFieldDescriptorBuilder::FromField(RFieldZero())
                           .FieldId(0)
                           .MakeDescriptor()
                           .Unwrap());
  auto descriptor = descBuilder.MoveDescriptor();

  RNTupleWriteOptions options;
  options.SetCompression(0);
  auto writer = RNTupleFileWriter::Recreate(
      "ntpl", filename, RNTupleFileWriter::EContainerFormat::kTFile,
      options);

  RNTupleSerializer serializer;

  auto ctx = serializer.SerializeHeader(nullptr, descriptor).Unwrap();
  auto buffer = std::make_unique<unsigned char[]>(ctx.GetHeaderSize());
  ctx = serializer.SerializeHeader(buffer.get(), descriptor).Unwrap();
  writer->WriteNTupleHeader(buffer.get(), ctx.GetHeaderSize(),
                            ctx.GetHeaderSize());

  auto szFooter = serializer.SerializeFooter(nullptr, descriptor, ctx).Unwrap();
  buffer = std::make_unique<unsigned char[]>(szFooter);
  serializer.SerializeFooter(buffer.get(), descriptor, ctx);
  writer->WriteNTupleFooter(buffer.get(), szFooter, szFooter);

  writer->Commit();
  // Call destructor to flush data to disk
  writer.reset();
}

# Compression

For the RNTuple Validation Suite, we assume that compression is orthogonal to the supported types and serialized data.
Therefore all tests in this category write a single `Int64` field with type `std::int64_t` and column type `SplitInt64`.
The entries have ascending values and the reference `.json` files only contain the sum of all elements.

 * [`algorithms`](algorithms): `zlib`, `lzma`, `lz4`, `zstd`
 * [`block`](block): big and short compression blocks

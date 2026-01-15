# Structure

For the RNTuple Validation Suite, we assume that the structure is orthogonal to the supported types and serialized data.
Therefore all tests in this category write a single `Int32` field with type `std::int32_t` the entries have ascending values.

 * [`clusters`](clusters): multiple clusters
 * [`cluster_groups`](cluster_groups): multiple cluster groups

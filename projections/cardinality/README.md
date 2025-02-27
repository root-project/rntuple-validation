# `ROOT::RNTupleCardinality`

## Fields

 * `[Split]Index{32,64}` of type `std::vector<std::int32_t>` with the corresponding column type for the offset column of the collection parent field
 * `[Split]Index{32,64}Cardinality` projected field of type `ROOT::RNTupleCardinality<std::uint{32,64}_t`

## Entries

1. Single-element vectors, with ascending values
2. Empty vectors
3. Increasing number of elements in the vector:
   one element in the first field, two elements in the second field, etc.

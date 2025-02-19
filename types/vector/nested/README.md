# `std::vector<std::vector<std::int32_t>>`

## Fields

 * `[Split]Index{32,64}`

with the corresponding column type for offset columns of the two collection parent fields.
All child fields use the default column encoding `Int32`.

## Entries

1. Single-element vectors, with ascending values
2. Empty vectors
3. Increasing number of elements in the outer vector, with arbitrary lengths of the inner vectors

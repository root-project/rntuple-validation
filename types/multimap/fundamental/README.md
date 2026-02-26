# `std::map<std::string, std::int32_t>`

## Fields

- `[Split]Index{32,64}`

with the corresponding column type for the offset column of the collection parent field.
All child fields use the default column types for `std::string` (`SplitIndex64` for the principal column, `Char` for the second column) and `std::int32_t` (`SplitInt32`).

## Entries

1. Single-element maps, with ascending values for both key and value
2. Empty maps
3. Increasing number of elements in the map:
   one key-value pair in the first field, two key-value pairs in the second field, etc.

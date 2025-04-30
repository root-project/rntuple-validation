# `std::optional`

## Fields

 * `[Split]Index{32,64}`: `std::optional<std::int32_t>`
   * with the corresponding column type for the first (principal) column
 * `String`: `std::optional<std::string>`
 * `Variant`: `std::optional<std::variant<std::int32_t, std::string>>`
 * `VectorInt32`: `std::optional<std::vector<std::int32_t>>`
 * `VectorOpt`: `std::vector<std::optional<std::int32_t>>>`

with otherwise the default column types.

## Entries

1. Simple values
2. No value
3. Zero / empty values

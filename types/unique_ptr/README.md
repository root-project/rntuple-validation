# `std::unique_ptr`

## Fields

 * `[Split]Index{32,64}`: `std::unique_ptr<std::int32_t>`
   * with the corresponding column type for the first (principal) column
 * `String`: `std::unique_ptr<std::string>`
 * `Variant`: `std::unique_ptr<std::variant<std::int32_t, std::string>>`
 * `VectorInt32`: `std::unique_ptr<std::vector<std::int32_t>>`
 * `VectorPtr`: `std::vector<std::unique_ptr<std::int32_t>>>`

with otherwise the default column types.

## Entries

1. Simple values
2. No value
3. Zero / empty values

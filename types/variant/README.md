# `std::variant`

## Fields

 * `f` of type `std::variant<std::int32_t, std::string, std::vector<std::int32_t>>`

## Entries

1. `std::int32_t` with value `1`
2. `std::string` with value `"abc"`
3. `std::vector<std::int32_t>` with value `{1, 2, 3}`
4. Empty `std::string`
5. Empty `std::vector`

__Missing:__
A `std::variant` that is `valueless_by_exception`.
This requires a type that throws during move assignment.

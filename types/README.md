# Types

 * [`array`](array): `std::array`
 * [`atomic`](atomic): `std::atomic`
 * [`bitset`](bitset): `std::bitset`
 * [`fundamental`](fundamental): fundamental column types
 * [`multiset`](multiset): `std::multiset` with all `[Split]Index{32,64}` column types
 * [`optional`](optional): `std::optional` with different element types
 * [`pair`](pair): `std::pair` with different element types
 * [`RVec`](RVec): `ROOT::RVec` with all `[Split]Index{32,64}` column types
 * [`set`](set): `std::set` with all `[Split]Index{32,64}` column types
 * [`string`](string): `std::string` with all `[Split]Index{32,64}` column types
 * [`tuple`](tuple): `std::tuple` with different element types
 * [`unique_ptr`](unique_ptr): `std::unique_ptr` with different element types
 * [`unordered_multiset`](unordered_multiset): `std::unordered_multiset` with all `[Split]Index{32,64}` column types
 * [`unordered_set`](unordered_set): `std::unordered_set` with all `[Split]Index{32,64}` column types
 * [`user`](user): user-defined types, such as classes and enums
 * [`variant`](variant): `std::variant` with `Switch` column type
 * [`vector`](vector): `std::vector` with all `[Split]Index{32,64}` column types

## Nesting of Types

In the RNTuple Validation Suite, we cannot test all possible nesting combinations of supported types.
Furthermore, we assume that the column encoding on disk is orthogonal to the nesting of fields.
We therefore cover a strategic selection based on field structural roles that is described below.
Implementations should exercise specific details related to nesting of types in their own unit tests.
In particular, this includes any optimization that contradicts the assumptions made above.

### Leaf Fields

For leaf fields, we test all possible column encodings in a top-level field.

### Collection Fields

For collection fields (for example `std::vector`), we want to test all `[Split]Index{32,64}` column types.

If `C` is a container type with a single item field, we test the following nestings:
 * `C<std::int32_t>`
 * `C<C<std::int32_t>>`

If `M` is an dictionary container type, we test the following nestings:
 * `M<std::int32_t, std::int32_t>`
 * `M<M<std::int32_t, std::int32_t>, M<std::int32_t, std::int32_t>>`

### Record Fields

For record fields of type `R` (for example `std::tuple`), we want to test nesting with other field structural roles:
 * Leaf fields of types `std::int32_t` and `std::string` (the latter is particular because it has variable length and two columns)
 * Variant field of type `std::variant<std::int32_t, std::string>`
 * Collection field of type `std::vector<std::int32_t>`
 * Record field of the type `R` itself

In addition, we test the nesting of the record type inside a collection field, `std::vector<R>`.

### Variant Fields

As there is only one type that results in this field structural role, the reader is referred to the [`variant`](variant) directory.

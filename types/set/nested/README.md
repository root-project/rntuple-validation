# `std::set<std::set<std::int32_t>>`

## Fields

 * `[Split]Index{32,64}`

with the corresponding column type for the offset column of the two collection parent fields.
All child fields use the default column encoding `Int32`.

## Entries

1. Single-element sets, with ascending values
2. Empty sets
3. Increasing number of elements in the outer set, with arbitrary lengths of the inner sets

## Dictionaries

These tests require ROOT dictionaries, which can be generated with the provided `Makefile` in this directory. This will create a `libNestedSet` shared object.

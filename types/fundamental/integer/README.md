# Integer Column Types

## Fields

 * `[U]Int{8,16,32,64}`
 * `Split[U]Int{16,32,64}`

with the corresponding column types.

## Entries

1. Ascending values
2. Values in each byte (to validate split encoding)
3. Negative values for signed integer types (to validate zigzag encoding)
4. Minimum (lowest) values
5. Maximum values

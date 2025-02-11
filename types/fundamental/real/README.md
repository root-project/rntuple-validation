# Real Column Types

Note that `Real32Trunc` and `Real32Quant` are separate.

## Fields

 * `FloatReal{16,32}`
 * `DoubleReal{16,32,64}`
 * `FloatSplitReal32`
 * `DoubleSplitReal{32,64}`

with the corresponding field and column types.

## Entries

1. Ascending values
2. Values in each byte (to validate split encoding)
3. Lowest values
4. Smallest positive normal values
5. Smallest positive subnormal values
6. Maximum values

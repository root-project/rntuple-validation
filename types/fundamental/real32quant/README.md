# Real Column Types with variable width and value range

## Fields

 * `FloatReal32Quant1`
 * `FloatReal32Quant8`
 * `FloatReal32Quant32` with value range `[-pi, pi]` (where `pi` is truncated to `float`)
 * `DoubleReal32Quant8`
 * `DoubleReal32Quant20`
 * `DoubleReal32Quant32` with value range `[-100, 25]`

with the corresponding field types, column type `Real32Quant`, with corresponding bit width
and a value range of `[-1, 1]` unless noted otherwise.

## Entries

1. Ascending values
2. All 1s in the mantissa
3. Middle values of the fields' value range
4. Min values of the fields' value range
5. Max values of the fields' value range

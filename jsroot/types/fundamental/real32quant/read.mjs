import { read, floatToHex } from "../../../jsroot_reader.mjs";

const fields = [
  "FloatReal32Quant1",
  "FloatReal32Quant8",
  "FloatReal32Quant32",
  "DoubleReal32Quant1",
  "DoubleReal32Quant20",
  "DoubleReal32Quant32",
];

const [input, output] = process.argv.slice(2);
read(input, output, fields, floatToHex);

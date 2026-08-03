import { read, floatToHex } from "../../../jsroot_reader.mjs";

const fields = [
  "FloatReal32Trunc10",
  "FloatReal32Trunc16",
  "FloatReal32Trunc31",
  "DoubleReal32Trunc10",
  "DoubleReal32Trunc16",
  "DoubleReal32Trunc31",
];

const [input, output] = process.argv.slice(2);
read(input, output, fields, floatToHex);

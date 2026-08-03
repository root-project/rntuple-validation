import { read, floatToHex } from "../../../jsroot_reader.mjs";

const fields = [
  "FloatReal16",
  "FloatReal32",
  "DoubleReal16",
  "DoubleReal32",
  "DoubleReal64",
  "FloatSplitReal32",
  "DoubleSplitReal32",
  "DoubleSplitReal64",
];

const [input, output] = process.argv.slice(2);
read(input, output, fields, floatToHex);

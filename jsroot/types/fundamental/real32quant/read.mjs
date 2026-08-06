import { read, floatToHex } from "../../../jsroot_reader.mjs";

function formatFloat(num, { field }) {
  // round num to single-precision float to match the required field precision
  if (field.startsWith("Float")) {
    num = Math.fround(num);
  }

  return floatToHex(num);
}

const fields = [
  "FloatReal32Quant1",
  "FloatReal32Quant8",
  "FloatReal32Quant32",
  "DoubleReal32Quant1",
  "DoubleReal32Quant20",
  "DoubleReal32Quant32",
];

const [input = "types.fundamental.real32quant.root", output = "types.fundamental.real32quant.json"] =
  process.argv.slice(2);

read(input, output, fields, formatFloat);

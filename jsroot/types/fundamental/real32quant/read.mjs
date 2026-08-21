import { read, floatToHex, isNewer } from "../../../jsroot_reader.mjs";
import { version } from "jsroot";

if (!isNewer(version, "7.11.1")) {
  console.log(" -> Skipped types/fundamental/real32quant: version too low")
  process.exit();
}

function formatFloat(num, { field }) {
  if (field.startsWith("Float")) {
    num = Math.fround(num); // round num to single-precision float to match the required field precision
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

const [
  input = "types.fundamental.real32quant.root",
  output = "types.fundamental.real32quant.json",
] = process.argv.slice(2);

read(input, output, fields, formatFloat);

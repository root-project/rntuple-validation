import { read, isNewer } from "../../../jsroot_reader.mjs";
import { version } from "jsroot";

if (!isNewer(version, "7.11.1")) {
  console.log(" -> Skipped types/fundamental/integer: version too low")
  process.exit();
}

function checkBigInt(value, { marker }) {
  const res =
    typeof value === "bigint" ? `${marker}${value}${marker}` : value;
  return res;
}

const fields = [
  "Int8",
  "UInt8",
  "Int16",
  "UInt16",
  "Int32",
  "UInt32",
  "Int64",
  "UInt64",
  "SplitInt16",
  "SplitUInt16",
  "SplitInt32",
  "SplitUInt32",
  "SplitInt64",
  "SplitUInt64",
];

const [
  input = "types.fundamental.integer.root",
  output = "types.fundamental.integer.json",
] = process.argv.slice(2);

read(input, output, fields, checkBigInt, { marker: "__BIGINT__" }); // marker is used to write BigInts as string in JSON and then convert to number to avoid precision loss

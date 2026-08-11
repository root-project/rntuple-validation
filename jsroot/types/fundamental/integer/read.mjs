import { read } from "../../../jsroot_reader.mjs";

/*
The following change in rntuple.mjs was necessary to make this test run:
1. line 910 & 919: remove Number() to avoid rounding of BigInt values
*/

function checkBigInt(value, { marker }) {
  const res = typeof value === "bigint" ? `${marker}${value}${marker}` : value;
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

const [input = "types.fundamental.integer.root", output = "types.fundamental.integer.json"] =
  process.argv.slice(2);

read(input, output, fields, checkBigInt, { marker: "__BIGINT__" }); // marker is used to write BigInts as string in JSON and then convert to number to avoid precision loss

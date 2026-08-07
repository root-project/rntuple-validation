import { read, floatToHex } from "../../jsroot_reader.mjs";

function normalizeValue(value, { marker, field }) {
  if (typeof value === "bigint") return `${marker}${value}${marker}`;
  if (typeof value === "string") return value.codePointAt(0);
  if (field.startsWith("Real")) return floatToHex(value, { field });
  return Number(value);
}

const fields = [
  "Bit",
  "Byte",
  "Char",
  "Int8",
  "UInt8",
  "Int16",
  "UInt16",
  "Int32",
  "UInt32",
  "Int64",
  "UInt64",
  "Real32",
  "Real64",
];

const [input = "types.atomic.root", output = "types.atomic.json"] =
  process.argv.slice(2);

read(input, output, fields, normalizeValue, { marker: "__BIGINT__" });

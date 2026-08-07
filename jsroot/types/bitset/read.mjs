import { read } from "../../jsroot_reader.mjs";

function bitsetToBinary(val, field) {
  const n = Number(field.field.replace("Bitset", ""));
  return BigInt(val).toString(2).padStart(n, "0");
}

const fields = ["Bitset1", "Bitset64", "Bitset65"];

const [input = "types.bitset.root", output = "types.bitset.json"] =
  process.argv.slice(2);

read(input, output, fields, bitsetToBinary);

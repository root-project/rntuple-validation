import { read } from "../../../jsroot_reader.mjs";

function convertToNum(value) {
  const res = typeof value === "string" ? value.codePointAt(0) : Number(value);
  return res;
}

const fields = ["Bit", "Byte", "Char"];

const [input, output] = process.argv.slice(2);
read(input, output, fields, convertToNum);

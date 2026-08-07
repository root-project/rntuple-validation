import { read } from "../../jsroot_reader.mjs";

const fields = [
  "Array_Int32",
  "Array_Array",
  "Array_String",
  "Array_Variant",
  "Array_Vector",
];

const [input = "types.array.root", output = "types.array.json"] =
  process.argv.slice(2);

read(input, output, fields);

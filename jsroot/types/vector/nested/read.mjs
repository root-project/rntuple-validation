import { read } from "../../../jsroot_reader.mjs";

const fields = ["Index32", "Index64", "SplitIndex32", "SplitIndex64"];

const [
  input = "types.vector.nested.root",
  output = "types.vector.nested.json",
] = process.argv.slice(2);

read(input, output, fields);

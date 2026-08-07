import { read } from "../../../jsroot_reader.mjs";

const fields = ["Index32", "Index64", "SplitIndex32", "SplitIndex64"];

const [
  input = "types.vector.fundamental.root",
  output = "types.vector.fundamental.json",
] = process.argv.slice(2);

read(input, output, fields);

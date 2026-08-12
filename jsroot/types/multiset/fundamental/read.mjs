import { read, sortArrayOfNumbers } from "../../../jsroot_reader.mjs";

const fields = ["Index32", "Index64", "SplitIndex32", "SplitIndex64"];

const [
  input = "types.multiset.fundamental.root",
  output = "types.multiset.fundamental.json",
] = process.argv.slice(2);

read(input, output, fields, sortArrayOfNumbers);

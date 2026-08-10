import { read, sortArrayOfNumbers } from "../../../jsroot_reader.mjs";

const fields = ["Index32", "Index64", "SplitIndex32", "SplitIndex64"];

const [
  input = "types.unordered_multiset.fundamental.root",
  output = "types.unordered_multiset.fundamental.json",
] = process.argv.slice(2);

read(input, output, fields, sortArrayOfNumbers);

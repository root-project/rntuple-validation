import { read, sortDeep } from "../../../jsroot_reader.mjs";

const fields = ["Index32", "Index64", "SplitIndex32", "SplitIndex64"];

const [
  input = "types.unordered_multiset.nested.root",
  output = "types.unordered_multiset.nested.json",
] = process.argv.slice(2);

read(input, output, fields, sortDeep);

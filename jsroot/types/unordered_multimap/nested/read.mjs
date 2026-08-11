import { read, pairArrayToMap } from "../../../jsroot_reader.mjs";

const fields = ["Index32", "Index64", "SplitIndex32", "SplitIndex64"];

const [
  input = "types.unordered_multimap.nested.root",
  output = "types.unordered_multimap.nested.json",
] = process.argv.slice(2);

read(input, output, fields, pairArrayToMap);

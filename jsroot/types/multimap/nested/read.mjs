import { read, pairArrayToMap } from "../../../jsroot_reader.mjs";

const fields = ["Index32", "Index64", "SplitIndex32", "SplitIndex64"];

const [
  input = "types.multimap.nested.root",
  output = "types.multimap.nested.json",
] = process.argv.slice(2);

read(input, output, fields, pairArrayToMap);

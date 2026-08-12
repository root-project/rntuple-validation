import { read } from "../../jsroot_reader.mjs";

const fields = [
  "Int32",
];

const [input = "structure.clusters.root", output = "structure.clusters.json"] =
  process.argv.slice(2);

read(input, output, fields);

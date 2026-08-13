import { read } from "../../jsroot_reader.mjs";

const fields = [
  "Int32",
];

const [input = "structure.cluster_groups.root", output = "structure.cluster_groups.json"] =
  process.argv.slice(2);

read(input, output, fields);

import { read } from "../../jsroot_reader.mjs";

const fields = ["f", "Vector"];

const [input = "types.variant.root", output = "types.variant.json"] =
  process.argv.slice(2);

read(input, output, fields);

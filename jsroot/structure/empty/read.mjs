import { read } from "../../jsroot_reader.mjs";

const fields = ["Int32"];

const [input = "structure.empty.root", output = "structure.empty.json"] =
  process.argv.slice(2);

read(input, output, fields);

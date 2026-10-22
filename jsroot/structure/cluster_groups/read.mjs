import { read, isNewer } from "../../jsroot_reader.mjs";

if (!isNewer("7.11.3")) {
  console.log(" -> Skipped structure/cluster_groups: version too low");
  process.exit();
}

const fields = ["Int32"];

const [
  input = "structure.cluster_groups.root",
  output = "structure.cluster_groups.json",
] = process.argv.slice(2);

read(input, output, fields);

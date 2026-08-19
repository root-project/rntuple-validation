import { read, isNewer } from "../../jsroot_reader.mjs";
import { version } from "jsroot";

if (!isNewer(version, "7.11.1")) {
  console.log(" -> Skipped structure/cluster_groups: version too low")
  process.exit();
}

const fields = ["Int32"];

const [
  input = "structure.cluster_groups.root",
  output = "structure.cluster_groups.json",
] = process.argv.slice(2);

read(input, output, fields);

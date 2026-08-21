import { read, isNewer } from "../../jsroot_reader.mjs";
import { version } from "jsroot";

if (!isNewer(version, "7.11.1")) {
  console.log(" -> Skipped structure/empty: version too low")
  process.exit();
}

const fields = ["Int32"];

const [input = "structure.empty.root", output = "structure.empty.json"] =
  process.argv.slice(2);

read(input, output, fields);

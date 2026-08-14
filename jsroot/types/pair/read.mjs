import { read } from "../../jsroot_reader.mjs";

function processPair(obj) {
  if (Array.isArray(obj)) return obj.map(processPair);
  if (obj && typeof obj === "object")
    return [processPair(obj.first), obj.second];
  return obj;
}

const fields = ["Int32_String", "Variant_Vector", "Pair", "VectorPair"];

const [input = "types.pair.root", output = "types.pair.json"] =
  process.argv.slice(2);

read(input, output, fields, processPair);

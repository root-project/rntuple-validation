import { read } from "../../jsroot_reader.mjs";

function tupleToArray(obj) {
  const listEntries = [];
  Object.values(obj).forEach((value) => {
    if (typeof value === "object") {
      value = tupleToArray(value);
    }
    listEntries.push(value);
  });
  return listEntries;
}

const fields = ["Int32_String_Vector", "Variant", "Tuple", "VectorTuple"];

const [input = "types.tuple.root", output = "types.tuple.json"] =
  process.argv.slice(2);

read(input, output, fields, tupleToArray);

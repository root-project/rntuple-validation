import { openFile, TSelector } from "jsroot";
import { rntupleProcess } from "jsroot/rntuple";
import { writeJson } from "../../../jsroot_reader.mjs";

async function read(
  input = "types.fundamental.misc.root",
  output = "types.fundamental.misc.json",
) {
  const file = await openFile(input),
    rntuple = await file.readObject("ntpl");
  
  let dict = [];

  // define fields that exist in .root file
  const selector = new TSelector(),
    fields = ["Bit", "Byte", "Char"];

  for (const f of fields) {
    selector.addBranch(f);
  }

  selector.Process = function (entryIndex) {
    const subdict = {};
    for (const field of fields) {
      try {
        const value = this.tgtobj[field];
        const res =
          typeof value === "string" ? value.codePointAt(0) : Number(value);
        subdict[field] = res;
      } catch (err) {
        console.error(
          `ERROR: Failed to read ${field} at entry ${entryIndex}: ${err.message}`,
        );
      }
    }
    dict.push(subdict);
  };

  await rntupleProcess(rntuple, selector);
  writeJson(dict, output);
}

const [input, output] = process.argv.slice(2);
read(input, output);

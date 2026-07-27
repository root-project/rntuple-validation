import { openFile, TSelector } from "jsroot";
import { rntupleProcess } from "jsroot/rntuple";
import { writeJson, floatToHex } from "../../../jsroot_reader.mjs";

async function read(
  input = "types.fundamental.real.root",
  output = "types.fundamental.real.json",
) {
  const file = await openFile(input),
    rntuple = await file.readObject("ntpl");
  
  let dict = [];

  // define fields that exist in .root file
  const selector = new TSelector(),
    fields = [
      "FloatReal16",
      "FloatReal32",
      "DoubleReal16",
      "DoubleReal32",
      "DoubleReal64",
      "FloatSplitReal32",
      "DoubleSplitReal32",
      "DoubleSplitReal64",
    ];

  for (const f of fields) {
    selector.addBranch(f);
  }

  selector.Process = function (entryIndex) {
    const subdict = {};
    for (const field of fields) {
      try {
        const value = this.tgtobj[field];
        subdict[field] = floatToHex(value);
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

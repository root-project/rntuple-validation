import { openFile, TSelector } from "jsroot";
import { rntupleProcess } from "jsroot/rntuple";
import { writeJson, floatToHex } from "../../../jsroot_reader.mjs";

async function read(
  input = "types.fundamental.real32quant.root",
  output = "types.fundamental.real32quant.json",
) {
  const file = await openFile(input),
    rntuple = await file.readObject("ntpl");
  
  let dict = [];

  // define fields that exist in .root file
  const selector = new TSelector(),
    fields = [
      "FloatReal32Quant1",
      "FloatReal32Quant8",
      "FloatReal32Quant32",
      "DoubleReal32Quant1",
      "DoubleReal32Quant20",
      "DoubleReal32Quant32",
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

import { openFile, TSelector } from "jsroot";
import { rntupleProcess } from "jsroot/rntuple";
import { writeJson } from "../../../jsroot_reader.mjs";

/*
The following change in rntuple.mjs was necessary to make this test run:
1. line 910 & 919: remove Number() to avoid rounding of BigInt values
*/

async function read(
  input = "types.fundamental.integer.root",
  output = "types.fundamental.integer.json",
) {
  const file = await openFile(input),
    rntuple = await file.readObject("ntpl"),
    marker = "__BIGINT__"; // used to write BigInts as string in JSON and then convert to number to avoid precision loss

  let dict = [];

  // define fields that exist in .root file
  const selector = new TSelector(),
    fields = [
      "Int8",
      "UInt8",
      "Int16",
      "UInt16",
      "Int32",
      "UInt32",
      "Int64",
      "UInt64",
      "SplitInt16",
      "SplitUInt16",
      "SplitInt32",
      "SplitUInt32",
      "SplitInt64",
      "SplitUInt64",
    ];

  for (const f of fields) {
    selector.addBranch(f);
  }

  selector.Process = function (entryIndex) {
    const subdict = {};
    for (const field of fields) {
      try {
        const value = this.tgtobj[field];
        const res =
          typeof value === "bigint" ? `${marker}${value}${marker}` : value;
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
  writeJson(dict, output, marker);
}

const [input, output] = process.argv.slice(2);
read(input, output);

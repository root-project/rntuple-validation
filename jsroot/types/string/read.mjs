import { read } from "../../jsroot_reader.mjs";

function getUtf8decoding(s) {
  const bytes = Uint8Array.from(s, (c) => c.charCodeAt(0));
  return new TextDecoder("utf-8").decode(bytes);
}

const fields = ["Index32", "Index64", "SplitIndex32", "SplitIndex64"];

const [input = "types.string.root", output = "types.string.json"] =
  process.argv.slice(2);

read(input, output, fields, getUtf8decoding);

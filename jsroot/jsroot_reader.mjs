import { writeFileSync, mkdirSync, existsSync } from 'fs';

export function writeJson(dict_input, output, marker=null) {
    let dict_string = JSON.stringify(dict_input, null, 2);
    dict_string += "\n"; // read.C macro have an empty new line at the end

    // create folder if not exist and output contains folder path
    let output_dir = output.split("/").slice(0, -1).join("/");
    if (output_dir !== "") {
        if (!existsSync(output_dir)){
            mkdirSync(output_dir, { recursive: true });
        }
    }

    // serialize BigInt as string and then remove the quotation marks, to avoid precision loss of Number()
    if (marker !== null) {
        dict_string = dict_string.replace(new RegExp(`"${marker}(-?\\d+)${marker}"`, 'g'),'$1');
    }

    writeFileSync(output, dict_string);
}

// convert float to hex representation based on IEEE-754 and C99 standard
export function floatToHex(num) {
  if (num === 0) return (Object.is(num, -0) ? "-" : "") + "0x0p+0";

  const buf = new ArrayBuffer(8); // 8 Byte == 64 Bit
  const view = new DataView(buf);
  view.setFloat64(0, num, false);

  const bits = view.getBigUint64(0, false); // bitwise operations only work with integer
  const sign = Number(bits >> 63n); // sign on first bit
  const rawExp = Number((bits >> 52n) & 0x7ffn); // exponent on next 11 bits
  const fraction = bits & 0xfffffffffffffn; // fraction on next 52 bits

  let exp, leading, mantBits;
  if (rawExp === 0) {
    // check for really small numbers
    if (fraction === 0n) return (sign ? "-" : "") + "0x0p+0";
    exp = -1022;
    leading = "0";
    mantBits = fraction;
  } else {
    exp = rawExp - 1023;
    leading = "1";
    mantBits = fraction;
  }

  let hex = mantBits.toString(16).padStart(13, "0").replace(/0+$/, "");
  const frac = hex ? "." + hex : "";
  const expStr = (exp >= 0 ? "+" : "-") + Math.abs(exp);

  return (sign ? "-" : "") + "0x" + leading + frac + "p" + expStr;
}

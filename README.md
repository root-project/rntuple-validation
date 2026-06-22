# RNTuple Validation Suite

The *RNTuple Validation Suite* provides conformance tests for the [RNTuple Binary Format Specification](https://cds.cern.ch/record/2923186).
The goal is to cover all parts of the specification with implications on the format.
It can be used to validate both writer and reader implementations of the RNTuple specification.
To that end, each test comes with a written description of the schema and the expected data.
Reference files for version 1.0.0 can be found under the [GitHub Release](https://github.com/root-project/rntuple-validation/releases/tag/v1.0.0).

## Test Categories

Tests in the RNTuple Validation Suite are organized into (nested) *categories*.
This is mirrored by the hierarchical directory layout in the repository.
For example, the [`types`](types) directory contains tests related to type support in the RNTuple specification.
It has subdirectories for tests concerning [fundamental types](types/fundamental) and C++ types, for example [`std::vector`](types/vector).
More tests are planned in the future, please [consult the list of issues](https://github.com/root-project/rntuple-validation/issues) in the GitHub repository.

## Reference Implementation

This repository also contains a reference implementation with ROOT macros.
They target the stable API released with ROOT v6.36 and should work on newer versions.
Compatibility with ROOT v6.34 can be tested with version v1.0 of the RNTuple Validation Suite.
It was the first official version of the RNTuple on-disk binary format, but the API was not yet finalized and all classes were in the `ROOT::Experimental` namespace.

### How to Run

For each test, we implement a `write.C` and `read.C` macro in the corresponding subdirectory.
The `write.C` macro produces a `.root` file with the contents as described in the `README` of each test subdirectory.
The `read.C` macro produces a `.json` file with a human-readable representation of the data in the `.root` file.
They can be run individually or all at once with `make` using the top-level [`Makefile`](Makefile).
The latter is also exercised by a GitHub Actions Workflow:
![Execute ROOT macros](https://github.com/root-project/rntuple-validation/actions/workflows/root.yml/badge.svg)
The job also uploads the produced set of `.root` and `.json` files, which can be downloaded from the Summary page.

Running the complete Validation Suite with `make` creates three folders:
- _dict_: contains the produced dictionaries (`.so`, `.pcm` and `.cxx` files)
- _write_: contains the produced `.root` files
- _read_: contains the produced `.json` files

Each operation can also be run individually, i.e. `make dict`, `make write` and `make read`. A ROOT version needs to be available beforehand!

To store results separately per version, subdirectories can be defined via the `dict_dir`, `write_dir` or `read_dir` arguments. Note that `read_dir` creates a subdirectory inside the folder named by `write_dir` within _read_. For example:
```
make dict_dir=6.38.00 write_dir=6.38.00 read_dir=6.38.00
```
produces the following structure:
```
dict/
├── 6.38.00/
    ├── .so
    ├── .pcm
    └── .cxx
write/
├── 6.38.00/
    └── .root
read/
├── 6.38.00/     <- version that wrote the .root files (write_dir)
    └── 6.38.00/ <- version that read them and produced the .json files (read_dir)
        └── .json
```
The two-level hierarchy in _read_ reflects the cross-validation between ROOT versions: the outer directory identifies the version whose `.root` files were used as input, and the inner directory identifies the version that read them and produced the `.json` output.

Use `make validate` to automatically run the validation suite across multiple versions. For that, set `source_scripts` to the paths to source for each version, separated by spaces. The dictionaries and binaries are then built for each version, and each ROOT version is tested against every subdirectory found in _write_. 

For example:
```
make validate source_dirs="<path_root_v1> <path_root_v2>"
```

Run `make download` in advance to download the ROOT and JSON [assets](https://github.com/root-project/rntuple-validation/releases/) from GitHub. Asset version `1.0.0` is used as default. Set the `ASSET_VERSION` argument to use another version.

### Exporting Results as HTML
Run `make export_html` to compare the generated `.json` files with each other and produce an `.html` file for visualization on a website. The output contains a table where each cell corresponds to a subfolder `read/<write_ver>/<read_ver>` and shows the comparison results against all other subfolders that satisfy the comparison logic described below. Clicking a cell reveals these results.

**Comparison logic**: Each subfolder is assigned a *limiting version*, defined as the smaller of its `write` and `read` versions. A subfolder is only compared against other subfolders whose limiting version is less than or equal to its own.

For example, `read/6.38.00/6.40.00` has a limiting version of `6.38.00` (the smaller of the two). It will be compared against `read/6.36.00/6.36.00` (limiting version `6.36.00` ≤ `6.38.00`), but not against `read/6.40.00/6.40.00` (limiting version `6.40.00` > `6.38.00`).

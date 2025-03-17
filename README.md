# RNTuple Validation Suite

The *RNTuple Validation Suite* provides conformance tests for the [RNTuple Binary Format Specification](https://cds.cern.ch/record/2923186).
The goal is to cover all parts of the specification with implications on the format.
It can be used to validate both writer and reader implementations of the RNTuple specification.
To that end, each test comes with a written description of the schema and the expected data.
Eventually, we will also provide a set of reference `.root` and `.json` files (see below).

> [!WARNING]
> The RNTuple Validation Suite is currently work-in-progress.
> Its structure and implementation may change during development.
> We welcome early feedback on the current state from any interested party.

## Test Categories

Tests in the RNTuple Validation Suite are organized into (nested) *categories*.
This is mirrored by the hierarchical directory layout in the repository.
For example, the [`types`](types) directory contains tests related to type support in the RNTuple specification.
It has subdirectories for tests concerning [fundamental types](types/fundamental) and C++ types, for example [`std::vector`](types/vector).
More tests are planned in the future, please [consult the list of issues](https://github.com/root-project/rntuple-validation/issues) in the GitHub repository.

## Reference Implementation

This repository also contains a reference implementation with ROOT macros.
They currently target ROOT v6.34 with the first official version of the on-disk binary format.
In this release, the API is not yet finalized and all classes are in the `ROOT::Experimental` namespace.
We plan to rely on this version until a first version of the RNTuple Validation Suite is done.
Then we will produce a first set of reference files that can be used to test backwards compatibility.
Afterwards, the implementation will be updated for the stable API (expected in ROOT v6.36).

### How to Run

For each test, we implement a `write.C` and `read.C` macro in the corresponding subdirectory.
The `write.C` macro produces a `.root` file with the contents as described in the `README` of each test subdirectory.
The `read.C` macro produces a `.json` file with a human-readable representation of the data in the `.root` file.
They can be run individually or all at once with `make` using the top-level [`Makefile`](Makefile).
The latter is also exercised by a GitHub Actions Workflow:
![Execute ROOT macros](https://github.com/root-project/rntuple-validation/actions/workflows/root.yml/badge.svg)
The job also uploads the produced set of `.root` and `.json` files, which can be downloaded from the Summary page.

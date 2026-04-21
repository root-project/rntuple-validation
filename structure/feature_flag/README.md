# Unknown feature flag set

An empty RNTuple with feature flag 137 (>62) set in the header. Reading should fail accordingly.
The error message should indicate the feature flag, as documented in the reference file `structure.feature_flag.json`.

A later version of this test should set the flag in the footer.

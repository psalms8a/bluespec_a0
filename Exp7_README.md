# Exp7 Polynomial Unit

This unit implements a 7-stage pipeline to approximate `exp(x)` for inputs in
`[-ln 2, ln 2]` using fixed-point math (`Fix32_8`).  Coefficients were fitted
using a Chebyshev approximation so that the maximum error is below `2^-20`.

## Simulation

A Bluesim test bench (`TestExp7.bsv`) drives at least `2^16` random vectors. A
C++ BDPI model (`exp7_ref.cpp`) computes the high precision reference.  Results
are compared cycle-by-cycle; the test prints `All tests passed` on success.

To run the test manually after installing BSC:

```bash
# build C++ helper
g++ -c -fPIC test/exp7_ref.cpp -o test/exp7_ref.o
# compile Bluespec
bsc -p +:src:test -u -sim -g mkTb test/TestExp7.bsv
bsc -sim -e mkTb -o mkTb test/mkTb.ba src/mkExp7.ba \
    test/exp_ref.ba test/rand_x.ba test/exp7_ref.o
# run simulation
./mkTb
```

The repository's test run used these steps and passed over 65k vectors.

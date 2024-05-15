## Demonstration inconsistent behavior among Verilog simulators

> Related to verilator issue [5036](https://github.com/verilator/verilator/issues/5036)

This repository demonstrates inconsistent behavior among Verilog simulators implementing VPI: Icarus (12.0), Modelsim SE(18.1), and Verilator (5.022). Specifically, when reading and writing logic values using the `vpiStringVal` value format.

Depending on the simulator's VPI implementation, individual bytes may be shifted, translated, sign extended, or truncated when read/written. The pattern of this behavior varies across multiple Verilog simulators.

These tests check for this edge case by attempting to read the values `0x0000`, `0xab00`, and `0x00ab` from Verilog registers using the `vpi_get_value` function. And writing `0xab00`, and `0x00ab` using `vpi_put_value`.

Each simulator test can be ran by executing `make clean && make` from within the corresponding subdirectory.

### Icarus output

```
verilog: test0=0000
verilog: test1=ab00
verilog: test2=00ab
verilog: test3=00ab
verilog: test4=0000

vpi:     test0=0030
vpi:     test1=ab20
vpi:     test2=ab00
vpi:     test3=ab00
vpi:     test4=0000
```

#### read observations
1. Trailing zero bytes are translated to 0x20 (ascii space character) when read.
2. Non-zero bytes following zero bytes are shifted left when read.
3. Values initialized to literal 0 are translated to 0x30 (ascii "0" character) when read.

#### write observations
1. Non-zero bytes preceding zero bytes are shifted right when written.
2. Non-zero bytes following zero bytes are truncated when written.

### Modelsim output

```
verilog: test0=0000
verilog: test1=ab00
verilog: test2=00ab
verilog: test3=ffab
verilog: test4=0000

vpi:     test0=2020
vpi:     test1=ab20
vpi:     test2=20ab
vpi:     test3=ffab
vpi:     test4=2020
```

#### read observations
1. Zero bytes are translated to 0x20 (ascii space character) when read.

#### write observations
1. Non-zero bytes preceding zero bytes are shifted right when written.
2. Non-zero bytes following zero bytes are truncated when written.
3. Values are implicitly sign-extended when written.

### Verilator output

```
vpi:     test0=2020
vpi:     test1=ab20
vpi:     test2=20ab
vpi:     test3=20ab
vpi:     test4=2020
verilog: test0=0000
verilog: test1=ab00
verilog: test2=00ab
verilog: test3=00ab
verilog: test4=0000
```
> Note vpi output is printed **before** verilog output.

#### read observations
1. Zero bytes are translated to 0x20 (ascii space character) when read.

#### write observations
1. Non-zero bytes preceding zero bytes are shifted right when written.
2. Non-zero bytes following zero bytes are truncated when written.

### Expected output

```
verilog: test0=0000
verilog: test1=ab00
verilog: test2=00ab
verilog: test3=ab00
verilog: test4=0000

vpi:     test0=0000
vpi:     test1=ab00
vpi:     test2=00ab
vpi:     test3=ab00
vpi:     test4=00ab
```

#### observations
1. Verilog output matches VPI output exactly.
2. No bytes are shifted, translated, sign-extended, or truncated when read or written.

### 

This repository demonstrates inconsistent behavior among Verilog simulators implementing VPI. Specifically, when reading or writing logic values via the `vpiStringVal` value format.

Depending on the simulator's VPI implementation, individual bytes may be shifted, translated, or ignored when read/written. The pattern of this behavior varies across multiple Verilog simulators.

These tests check for this edge case by attempting to read the values `0x0000`, `0xab00`, and `0x00ab` from Verilog registers using the `vpi_get_value` function. And writing `0xab00`, and `0x00ab` using `vpi_put_value`.

Each simulator test can be ran by executing `make clean && make` from within the corresponding subdirectory. **Note**, the respective simulator must be discoverable in your PATH.

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

Trailing 0s are translated to 0x20 when read.
Bytes following leading 0s are shifted left when read.
Values initialized to 0 are translated to 0x30 `'0'` when read.

Bytes preceding trailing 0s are shifted right when written.
Bytes following leading 0s are ignored when written.

### Verilator output
```
verilog: test0=0000
verilog: test1=ab00
verilog: test2=00ab
verilog: test3=00ab
verilog: test4=0000

vpi:     test0=2020
vpi:     test1=ab20
vpi:     test2=20ab
vpi:     test3=20ab
vpi:     test4=2020
```

All 0s are translated to 0x20 when read.

Bytes preceding trailing 0s are shifted right when written.
Bytes following leading 0s are ignored when written.

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

Verilog output matches vpi output **exactly**

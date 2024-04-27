.PHONY: all clean

all: top.vvp top.vpi
	vvp -m top -M. $^

top.vpi: top.c
	iverilog-vpi $^

top.vvp: top.sv
	iverilog -g 2012 -o$@ $^

clean:
	rm -f *.vvp *.vpi *.o

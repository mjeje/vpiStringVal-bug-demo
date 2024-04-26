.PHONY: all clean

all: top.vvp top.vpi
	vvp -m top -M. $^

top.vpi: top.c
	iverilog-vpi $^

top.vvp: top.v
	iverilog -o$@ $^

clean:
	rm -f *.vvp *.vpi *.o

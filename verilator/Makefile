.PHONY: all clean

all: obj_dir/Vtop
	./obj_dir/Vtop

obj_dir/Vtop: ../src/top.c ../src/top.sv
	verilator --cc --exe --build --vpi --public-flat-rw -CFLAGS "-DVERILATOR=1 -Wall -Wno-write-strings" $^

clean:
	rm -rf obj_dir

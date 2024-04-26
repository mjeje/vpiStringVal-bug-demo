.PHONY: all clean

all: obj_dir/Vtop
	./obj_dir/Vtop

obj_dir/Vtop: top.cc top.v
	verilator --cc --exe --build --vpi --public-flat-rw -j 0 -Wall $^

clean:
	rm -rf obj_dir

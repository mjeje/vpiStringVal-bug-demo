.PHONY: all clean

all: ../src/top.sv ../src/top.c run.do
	vsim -c -do run.do

clean:
	rm -rf *.so *.o *.vstf work transcript

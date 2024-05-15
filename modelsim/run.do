onerror {resume}

if [file exists work] {
    vdel -all
}

# Create the library.
vlib work

# Get the simulator installation directory.
quietly set INSTALL_HOME [file dirname [file nativename $::env(MODEL_TECH)]] 

# Compile the HDL source(s)
vlog ../src/top.sv

# Compile the C source(s).
quietly set CC "gcc -m32 -c -B/usr/lib/i386-linux-gnu -I$INSTALL_HOME/include"
quietly set LD "gcc -m32 -shared -B/usr/lib/i386-linux-gnu -o"
quietly set MTIPLILIB $INSTALL_HOME/linux/libmtipli.so
quietly set ext so

echo $CC ../src/top.c
eval $CC ../src/top.c
echo $LD top.$ext top.o $MTIPLILIB
eval $LD top.$ext top.o $MTIPLILIB


# Simulate the design.
vsim -c top -pli top.$ext
run -all
quit -f

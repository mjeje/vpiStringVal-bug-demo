#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "Vtop.h"
#include "verilated_vpi.h"
#include "verilated.h"

static u_int16_t get_value(char* name)
{
    vpiHandle handle = vpi_handle_by_name(name, NULL);
    assert(handle != NULL);

    s_vpi_value value;
    value.format = vpiStringVal;
    vpi_get_value(handle, &value);

    u_int16_t uintVal = 0;
    uintVal |= (value.value.str[0] & 255) << 8;
    uintVal |= value.value.str[1] & 255;

    return uintVal;
}

static void set_value(char* name, char* value)
{
    vpiHandle handle = vpi_handle_by_name(name, NULL);
    assert(handle != NULL);

    s_vpi_value value_s;
    value_s.format = vpiStringVal;
    value_s.value.str = value;

    s_vpi_time time;
    time.type = vpiSimTime;
    time.high = 0;
    time.low  = 1;

    vpi_put_value(handle, &value_s, &time, vpiInertialDelay);
}

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp,""};
    contextp->internalsDump();
    top->eval();

    char test3str[3] = {0x20,0x00,0x00};
    char test4str[3] = {0x00,0x20,0x00};
    set_value("top.test3", test3str);
    set_value("top.test4", test4str);

    top->eval();

    u_int16_t test1 = get_value("top.test1");
    u_int16_t test2 = get_value("top.test2");
    u_int16_t test3 = get_value("top.test3");
    u_int16_t test4 = get_value("top.test4");

    top->final();
    delete top;
    delete contextp;

    printf("vpi: test1=%04x\n",test1);
    printf("vpi: test2=%04x\n",test2);
    printf("vpi: test3=%04x\n",test3);
    printf("vpi: test4=%04x\n",test4);

    assert(test1 == 0x2000);
    assert(test2 == 0x0020);
    assert(test3 == 0x2000);
    assert(test4 == 0x0020);

    return 0;
}
#include "vpi_user.h"

#define UNUSED(arg) (void)(arg)

static uint16_t get_value(PLI_BYTE8* name)
{
    vpiHandle handle = vpi_handle_by_name(name, 0);

    s_vpi_value value;
    value.format = vpiStringVal;
    vpi_get_value(handle, &value);

    uint16_t uintVal = 0;
    uintVal |= (value.value.str[0] & 255) << 8;
    uintVal |= value.value.str[1] & 255;

    return uintVal;
}

static void set_value(PLI_BYTE8* name, PLI_BYTE8* value)
{
    vpiHandle handle = vpi_handle_by_name(name, 0);

    s_vpi_value value_s;
    value_s.format = vpiStringVal;
    value_s.value.str = value;

    s_vpi_time time;
    time.type = vpiSimTime;
    time.high = 0;
    time.low  = 1;

    vpi_put_value(handle, &value_s, &time, vpiInertialDelay);
}

static int time_cb(p_cb_data cb_data)
{
    UNUSED(cb_data);

    //string is null terminated
    PLI_BYTE8 test3val[3] = {'\xab','\x00','\x00'};
    PLI_BYTE8 test4val[3] = {'\x00','\xab','\x00'};
    
    set_value("top.test3", test3val);
    set_value("top.test4", test4val);

    return 0;
}

static int final_cb(p_cb_data cb_data)
{
    UNUSED(cb_data);

    uint16_t test0 = get_value("top.test0");
    uint16_t test1 = get_value("top.test1");
    uint16_t test2 = get_value("top.test2");
    uint16_t test3 = get_value("top.test3");
    uint16_t test4 = get_value("top.test4");

    vpi_printf("vpi:     test0=%04x\n",test0);
    vpi_printf("vpi:     test1=%04x\n",test1);
    vpi_printf("vpi:     test2=%04x\n",test2);
    vpi_printf("vpi:     test3=%04x\n",test3);
    vpi_printf("vpi:     test4=%04x\n",test4);

    return 0;
}

static void register_time_cb(void)
{
    s_cb_data cb_data = {0};
    cb_data.reason    = cbNextSimTime;
    cb_data.cb_rtn    = time_cb;

    vpi_register_cb(&cb_data);
}

static void register_final_cb(void)
{
    s_cb_data cb_data = {0};
    cb_data.reason    = cbEndOfSimulation;
    cb_data.obj       = 0;
    cb_data.cb_rtn    = final_cb;

    vpi_register_cb(&cb_data);
}

static int simulation_started_cb(p_cb_data cb_data)
{
    UNUSED(cb_data);

    register_time_cb();
    register_final_cb();

    return 0;
}

static void startup_routine(void)
{
    s_cb_data cb_data = {0};
    cb_data.reason    = cbStartOfSimulation;
    cb_data.cb_rtn    = simulation_started_cb;
    
    vpi_register_cb(&cb_data);
}

void (*vlog_startup_routines[])(void) = 
{
    startup_routine,
    0
};

#ifdef VERILATOR
#include "Vtop.h"
int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp,""};
    top->eval();

    time_cb(0);
    top->eval();

    final_cb(0);
    top->final();
    delete top;
    delete contextp;

    return 0;
}
#endif
#include <assert.h>
#include <string.h>
#include <vpi_user.h>
#include <stdio.h>

static u_int16_t get_value(const char* name)
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

static void set_value(const char* name, char* value)
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

static int clock_value_changed_cb(p_cb_data cb_data)
{
    if (cb_data->value->value.integer != 1) {
        //strings end with null terminator
        char test3[3] = {0x20,0x00,0x00};
        char test4[3] = {0x00,0x20,0x00};
        
        set_value("top.test3", test3);
        set_value("top.test4", test4);
    }else {
        u_int16_t test1 = get_value("top.test1");
        u_int16_t test2 = get_value("top.test2");
        u_int16_t test3 = get_value("top.test3");
        u_int16_t test4 = get_value("top.test4");

        printf("vpi: test1=%04x\n",test1);
        printf("vpi: test2=%04x\n",test2);
        printf("vpi: test3=%04x\n",test3);
        printf("vpi: test4=%04x\n",test4);

        assert(test1 == 0x2000);
        assert(test2 == 0x0020);
        assert(test3 == 0x2000);
        assert(test4 == 0x0020);
    }
}

static void register_clock_change_cb()
{
    s_cb_data cb_data = {0};
    cb_data.reason    = cbValueChange;
    cb_data.obj       = vpi_handle_by_name("top.clk", NULL);
    cb_data.cb_rtn    = clock_value_changed_cb;

    s_vpi_value value = {0};
    value.format      = vpiIntVal;
    cb_data.value     = &value;

    vpi_register_cb(&cb_data);
}

static int simulation_started_cb(p_cb_data cb_data)
{
    register_clock_change_cb();
}

static void startup_routine(void)
{
    s_cb_data cb_data = {0};
    cb_data.reason    = cbStartOfSimulation;
    cb_data.cb_rtn    = simulation_started_cb;
    
    vpi_register_cb(&cb_data);
}

void (*vlog_startup_routines[])() = 
{
    startup_routine,
    0
};

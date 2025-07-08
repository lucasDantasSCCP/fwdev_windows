#include "hal.h"
#include "app.h"

static uint8_t dev_id[HAL_CPU_ID_SIZE + 1];
static uint32_t init_time_ms;

void app_init(void)
{
    #if 0
    utl_dbg_mod_enable(UTL_DBG_MOD_APP);
    UTL_DBG_PRINTF(UTL_DBG_MOD_APP, "Initalizing app...\n");

    hal_cpu_id_get(dev_id);
    dev_id[HAL_CPU_ID_SIZE] = '\0'; 
    UTL_DBG_PRINTF(UTL_DBG_MOD_APP, "Device ID: %s\n", dev_id);

    init_time_ms = hal_cpu_time_get_ms();
    #endif

    hal_serial_init();
    hal_serial_open((uint8_t*)"COM1");
}

bool app_loop(void)
{
    #if 0 
    hal_cpu_watchdog_refresh();
    hal_cpu_sleep_ms(1000);
    uint32_t elapsed_time_ms = hal_cpu_time_elapsed_get_ms(init_time_ms);
    UTL_DBG_PRINTF(UTL_DBG_MOD_APP, "App running, cpu time: %ums\n", elapsed_time_ms);

    if(elapsed_time_ms > 10000)
    {
        UTL_DBG_PRINTF(UTL_DBG_MOD_APP, "App running for more than 10 seconds, resetting...\n");
        hal_cpu_reset();
    }
    
    return !app_terminate_get();
    #endif
}

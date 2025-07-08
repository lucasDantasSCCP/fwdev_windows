#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "hal.h"
#include "app.h"

extern char *main_app_name_get(void);

static bool port_cpu_init_done = false;
static CRITICAL_SECTION port_cpu_cs;

static HANDLE systick_thread = NULL;
static volatile uint32_t port_systick_cnt = 0;
static bool systick_thread_running = false;

BOOL WINAPI port_cpu_ctrl_handler(DWORD fdwCtrlType)
{
    if (fdwCtrlType == CTRL_C_EVENT) {
        printf("[PORT] CTRL+C pressed!\n");
        app_terminate_set();
        return TRUE;
    }
    return FALSE;
}

DWORD WINAPI port_systick_thread(LPVOID lpParam)
{
    while (systick_thread_running) {
        Sleep(1); // 1ms delay
        port_systick_cnt++;
    }
    return 0;
}

static void port_systick_create(void)
{
    systick_thread_running = true;
    systick_thread = CreateThread(NULL, 0, port_systick_thread, NULL, 0, NULL);

    if (!systick_thread) {
        printf("[PORT] Failed to create SysTick thread!\n");
    }
}

static void port_cpu_init(void)
{
    printf("[PORT] Creating critical section mutex...\n");
    InitializeCriticalSection(&port_cpu_cs);

    printf("[PORT] Setting CTRL+C handler...\n");
    SetConsoleCtrlHandler(port_cpu_ctrl_handler, TRUE);

    printf("[PORT] Initializing 1ms SysTick thread...\n");
    port_systick_create();

    port_cpu_init_done = true;
}

static void port_cpu_deinit(void)
{
    systick_thread_running = false;
    if (systick_thread) {
        WaitForSingleObject(systick_thread, INFINITE);
        CloseHandle(systick_thread);
    }
    DeleteCriticalSection(&port_cpu_cs);
}

static void port_cpu_reset(void)
{
    char *app_name = main_app_name_get();
    char *rst_cmd = malloc(strlen(app_name) + 64);

    printf("[PORT] Restarting app %s in 5s...\n", app_name);
    sprintf(rst_cmd, "cmd /C \"timeout /T 5 & start %s\"", app_name);

    system(rst_cmd);
    free(rst_cmd);

    app_terminate_set(); 
}

static void port_cpu_watchdog_refresh(void) { }

static void port_cpu_id_get(uint8_t *id)
{
    memcpy(id, "WIN_SIM_CPU", HAL_CPU_ID_SIZE);
}

static uint32_t port_cpu_random_seed_get(void)
{
    time_t tm = time(NULL);
    uint32_t seed = (uint32_t)tm;
    printf("[PORT] Random seed: 0x%08X\n", seed);
    return seed;
}

static void port_cpu_critical_section_leave(uint32_t last_level)
{
    LeaveCriticalSection(&port_cpu_cs);
}

static uint32_t port_cpu_critical_section_enter(hal_cpu_cs_level_t level)
{
    EnterCriticalSection(&port_cpu_cs);
    return 0;
}

static void port_cpu_low_power_enter(void) { }

static uint32_t port_cpu_time_get_ms(void)
{
    return port_systick_cnt;
}

static void port_cpu_sleep_ms(uint32_t tmr_ms)
{
    Sleep(tmr_ms);
}

hal_cpu_driver_t HAL_CPU_DRIVER =
{
    .init = port_cpu_init,
    .deinit = port_cpu_deinit,
    .reset = port_cpu_reset,
    .watchdog_refresh = port_cpu_watchdog_refresh,
    .id_get = port_cpu_id_get,
    .random_seed_get = port_cpu_random_seed_get,
    .critical_section_enter = port_cpu_critical_section_enter,
    .critical_section_leave = port_cpu_critical_section_leave,
    .low_power_enter = port_cpu_low_power_enter,
    .sleep_ms = port_cpu_sleep_ms,
    .time_get_ms = port_cpu_time_get_ms,

};


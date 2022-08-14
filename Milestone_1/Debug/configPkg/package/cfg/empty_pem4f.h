/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-B06
 */

#include <xdc/std.h>

#include <ti/sysbios/hal/Hwi.h>
extern const ti_sysbios_hal_Hwi_Handle uart_hwi;

#include <ti/sysbios/knl/Swi.h>
extern const ti_sysbios_knl_Swi_Handle uart_swi;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle audioTask;

#include <ti/sysbios/knl/Swi.h>
extern const ti_sysbios_knl_Swi_Handle adc_swi;

#include <ti/sysbios/hal/Hwi.h>
extern const ti_sysbios_hal_Hwi_Handle adc_hwi;

#include <ti/sysbios/knl/Clock.h>
extern const ti_sysbios_knl_Clock_Handle clock0;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle sendErrorData;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle BufferAvailableSema;

extern int xdc_runtime_Startup__EXECFXN__C;

extern int xdc_runtime_Startup__RESETFXN__C;


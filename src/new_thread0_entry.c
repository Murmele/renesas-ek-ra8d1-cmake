#include "new_thread0.h"
#include "ospi_flash.h"
#include "ospi_common.h"
#include "touch_GT911.h"
#include "read_GT911_touch.h"

touch_data_t g_touch_data;

void glcdc_callback(display_callback_args_t * p_args)
{
    if(DISPLAY_EVENT_LINE_DETECTION == p_args->event) {
        __NOP();
    }
    else if(DISPLAY_EVENT_GR1_UNDERFLOW == p_args->event) {
        __BKPT(0); /*Layer 1 Underrun*/
    }
    else if(DISPLAY_EVENT_GR2_UNDERFLOW == p_args->event) {
        __BKPT(0); /*Layer 2 Underrun*/
    }
    else { /*DISPLAY_EVENT_FRAME_END*/
        __BKPT(0);
    }
}

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 854

#define RGB_565_RED    (0x1F << 11)
#define RGB_565_GREEN  (0x3F << 5)
#define RGB_565_BLUE   (0x1F << 0)

void slint_main();

/* New Thread entry function */
/* pvParameters contains TaskHandle_t */
void new_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);
    fsp_err_t err;
    uint16_t * p = (uint16_t *)&fb_background[0][0];
    uint16_t i;
    uint16_t j;

    ospi_reset();

    init_opsi_flash();

    ospi_change_to_max_speed();

    /* Open the I2C bus if it is not already open. */
    rm_comms_i2c_bus_extended_cfg_t * p_extend          = (rm_comms_i2c_bus_extended_cfg_t *) g_comms_i2c_device0_cfg.p_extend;
    i2c_master_instance_t           * p_driver_instance = (i2c_master_instance_t *) p_extend->p_driver_instance;

    err = p_driver_instance->p_api->open(p_driver_instance->p_ctrl, p_driver_instance->p_cfg);
    assert(FSP_SUCCESS == err);

#if (2 == BSP_CFG_RTOS)
    /* Create a semaphore for blocking if a semaphore is not NULL */
    if (NULL != p_extend->p_blocking_semaphore)
    {
              // FreeRTOS
        *(p_extend->p_blocking_semaphore->p_semaphore_handle) =
            xSemaphoreCreateCountingStatic((UBaseType_t) 1,
                                           (UBaseType_t) 0,
                                           p_extend->p_blocking_semaphore->p_semaphore_memory);
    }
    /* Create a recursive mutex for bus lock if a recursive mutex is not NULL */
    if (NULL != p_extend->p_bus_recursive_mutex)
    {
            // FreeRTOS
        *(p_extend->p_bus_recursive_mutex->p_mutex_handle) =
            xSemaphoreCreateRecursiveMutexStatic(p_extend->p_bus_recursive_mutex->p_mutex_memory);
    }
#endif
    /* Need to initialise the Touch Controller before the LCD, as only a Single Reset line shared between them */
    /*
        err =
            RM_COMMS_I2C_Open(&g_comms_i2c_device0_ctrl,
       &g_comms_i2c_device0_cfg); assert(FSP_SUCCESS == err);

        err = init_ts(&g_comms_i2c_device0_ctrl);
        assert(FSP_SUCCESS == err);

        err = enable_ts(&g_comms_i2c_device0_ctrl);
        assert(FSP_SUCCESS == err);
        */

    for (j = 0; j < DISPLAY_HEIGHT / 2; j++) {
        for (i = 0; i < DISPLAY_WIDTH/2; i++)
        {
            *p++ = RGB_565_BLUE;
        }

        for (i = 0; i < DISPLAY_WIDTH/2; i++)
        {
            *p++ = RGB_565_RED;
        }
    }

    for (j = 0; j < DISPLAY_HEIGHT/2; j++)
    {
        for (i = 0; i < DISPLAY_WIDTH/2; i++)
        {
            *p++ = RGB_565_GREEN;
        }

        for (i = 0; i < DISPLAY_WIDTH/2; i++)
        {
            *p++ = (RGB_565_RED | RGB_565_GREEN | RGB_565_BLUE);
        }
    }
    /*
    err = R_GLCDC_Open(&g_display0_ctrl, &g_display0_cfg);
    assert(FSP_SUCCESS == err);

    err = R_GLCDC_Start(&g_display0_ctrl);
    assert(FSP_SUCCESS == err);

    do {
        err = R_GLCDC_BufferChange(&g_display0_ctrl, &fb_background[0][0],
                                   (display_frame_layer_t)0);
    } while (FSP_ERR_INVALID_UPDATE_TIMING == err);
            */
    // Turn on the Backlight, could do it with a PWM for dimming,
    // this is simpler
    R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_BLEN, BSP_IO_LEVEL_HIGH);

    /* TODO: add your own code here */
    slint_main();
}

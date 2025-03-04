#include <new_thread0.h>
#include <touch_GT911.h>

#define I2C_TRANSFER_COMPLETE  (1<<0)
#define I2C_TRANSFER_ABORT     (1<<1)

#define I2C_TIMEOUT_MS         1000/portTICK_PERIOD_MS

static fsp_err_t productId(rm_comms_ctrl_t * p_api_ctrl, uint8_t *target);
static void reset_gt911(void);

void comms_i2c_callback(rm_comms_callback_args_t *p_args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFAIL;

    if (RM_COMMS_EVENT_OPERATION_COMPLETE == p_args->event)
    {
        xResult = xEventGroupSetBitsFromISR(g_i2c_event_group, I2C_TRANSFER_COMPLETE, &xHigherPriorityTaskWoken );
    }
    else if (RM_COMMS_EVENT_ERROR == p_args->event)
    {
        xResult = xEventGroupSetBitsFromISR(g_i2c_event_group, I2C_TRANSFER_ABORT, &xHigherPriorityTaskWoken );
    }
    else
    {
       //should never get here.
    }

    /* Was the message posted successfully? */
    if( pdFAIL != xResult)
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested.  The macro used is port specific and will
        be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
        the documentation page for the port being used. */
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }

}

fsp_err_t i2c_cb_wait(void)
{
    fsp_err_t ret = FSP_SUCCESS;
    EventBits_t uxBits;

    uxBits =  xEventGroupWaitBits(g_i2c_event_group,
                    I2C_TRANSFER_COMPLETE | I2C_TRANSFER_ABORT,
                    pdTRUE, //Clearbits before returning
                    pdFALSE, //either bit will do
                    I2C_TIMEOUT_MS  );

    if ((I2C_TRANSFER_COMPLETE & uxBits) == I2C_TRANSFER_COMPLETE)
    {
        ret = FSP_SUCCESS;
    }
    else if ((I2C_TRANSFER_ABORT & uxBits) == I2C_TRANSFER_ABORT)
    {
        ret = FSP_ERR_ABORTED;
    }
    else
    {
        /* xEventGroupWaitBits() returned because of timeout */
        ret = FSP_ERR_TIMEOUT;
    }

    return ret;
}

/**********************************************************************************************************************
 * Function definitions
 **********************************************************************************************************************/

static void reset_gt911(void)
{
        R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_RST, BSP_IO_LEVEL_LOW);

        R_IOPORT_PinCfg(&g_ioport_ctrl, DISP_INT,  (uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT | (uint32_t) IOPORT_CFG_PORT_OUTPUT_LOW);

        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);

        R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_INT, BSP_IO_LEVEL_LOW); //Address 0x5D

        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);

        R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_RST, BSP_IO_LEVEL_HIGH);

        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);

        R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_INT, BSP_IO_LEVEL_LOW);

        R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);

        R_IOPORT_PinCfg(&g_ioport_ctrl, DISP_INT,  ((uint32_t) IOPORT_CFG_IRQ_ENABLE | (uint32_t) IOPORT_CFG_PORT_DIRECTION_INPUT));

        R_IOPORT_PinCfg(&g_ioport_ctrl, DISP_RST,  (uint32_t) IOPORT_CFG_PORT_DIRECTION_INPUT);
}


static fsp_err_t productId(rm_comms_ctrl_t * p_api_ctrl, uint8_t *target)
{
    fsp_err_t err;

    rm_comms_write_read_params_t write_read_params;

    uint8_t reg_id[2];
    reg_id[0] = (GT911_REG_PRODUCT_ID & 0xFF00) >> 8;
    reg_id[1] = (GT911_REG_PRODUCT_ID & 0x00FF);

    write_read_params.p_dest     = target;
    write_read_params.dest_bytes = 4;
    write_read_params.p_src      = reg_id;
    write_read_params.src_bytes  = 2;

    err = RM_COMMS_I2C_WriteRead(p_api_ctrl, write_read_params);//;(p_api_ctrl, GT911_REG_PRODUCT_ID, (uint8_t *)target, 4);
    if (FSP_SUCCESS == err)
    {
        err = i2c_cb_wait();
    }

    return (err);
}


fsp_err_t init_ts(rm_comms_ctrl_t * p_api_ctrl)
{
    fsp_err_t err;
    uint8_t product_id[5];

    reset_gt911();

    err = productId(p_api_ctrl, &product_id[0]);
    if (FSP_SUCCESS == err)
    {
        if (product_id[0] != '9')
        {
            /* Product ID should be 9xx */
            err = FSP_ERR_ASSERTION;
        }
    }

    return(err);    // product id should be 911
}


fsp_err_t enable_ts(rm_comms_ctrl_t * p_api_i2c_ctrl)
{

    fsp_err_t err;
    uint8_t product_id[5];

    reset_gt911();

    err = productId(p_api_i2c_ctrl, &product_id[0]);
    if (FSP_SUCCESS == err)
    {
        if (product_id[0] != '9')
        {
            /* Product ID should be 9xx */
            err = FSP_ERR_ABORTED;
        }
    }

    if (FSP_SUCCESS == err)
    {
        uint8_t enable_touch[3];

        enable_touch[0] = (GT911_REG_COMMAND & 0xFF00) >> 8;
        enable_touch[1] = GT911_REG_COMMAND & 0x00FF;
        enable_touch[2] = 0;

        err = RM_COMMS_I2C_Write(p_api_i2c_ctrl, enable_touch, 3);
        if (FSP_SUCCESS == err)
        {
            err = i2c_cb_wait();
        }
    }


    return err;
}

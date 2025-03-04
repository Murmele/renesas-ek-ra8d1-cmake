#include "new_thread0.h"
#include "touch_GT911.h"
#include "read_GT911_touch.h"

void touchpad_read(touch_data_t * data)
{
    fsp_err_t err;
    uint32_t number_of_coordinates;
    uint8_t read_data[7];
    uint8_t write_data[3];

    TouchCordinate_t coordinates[6];
    memset(coordinates, 0, sizeof(TouchCordinate_t));

    rm_comms_write_read_params_t write_read_params;

    write_data[0] = (GT911_REG_READ_COORD_ADDR & 0xFF00) >> 8;
    write_data[1] = (GT911_REG_READ_COORD_ADDR & 0x00FF);
    write_data[2] = 0;

    write_read_params.p_dest     = read_data;
    write_read_params.dest_bytes = 1;
    write_read_params.p_src      = write_data;
    write_read_params.src_bytes  = 2;

    err = RM_COMMS_I2C_WriteRead(&g_comms_i2c_device0_ctrl, write_read_params);
    if (FSP_SUCCESS == err)
    {
        err = i2c_cb_wait();
    }
    assert(FSP_SUCCESS == err);

    if (BUFFER_READY == (BUFFER_READY & read_data[0]))
    {
        number_of_coordinates = (read_data[0] & NUM_TOUCH_POINTS_MASK);
        if (number_of_coordinates != 0)
        {
            for (uint8_t i = 0; i < number_of_coordinates; i++)
            {
                uint16_t reg = (GT911_REG_POINT1_X_ADDR + (i * 8));

                write_data[0] = (reg & 0xFF00) >> 8;
                write_data[1] = (reg & 0x00FF);
                write_data[2] = 0;

                write_read_params.p_dest     = read_data;
                write_read_params.dest_bytes = 7;
                write_read_params.p_src      = write_data;
                write_read_params.src_bytes  = 2;

                err = RM_COMMS_I2C_WriteRead(&g_comms_i2c_device0_ctrl, write_read_params);
                if (FSP_SUCCESS == err)
                {
                    err = i2c_cb_wait();
                }
                assert(FSP_SUCCESS == err);

                coordinates[i].x = (int32_t)((read_data[2] << 8) | read_data[1]);
                coordinates[i].y = (int32_t)((read_data[4] << 8) | read_data[3]);

            }
            data->state = TOUCH_STATE_PRESSED;
        }
        else
        {
            data->state = TOUCH_STATE_RELEASED;
        }

        /* Set status to 0, to wait for next touch event */
        write_data[0] = (GT911_REG_READ_COORD_ADDR & 0xFF00) >> 8;
        write_data[1] = (GT911_REG_READ_COORD_ADDR & 0x00FF);
        write_data[2] = 0;

        err = RM_COMMS_I2C_Write(&g_comms_i2c_device0_ctrl, write_data, 3);
        if (FSP_SUCCESS == err)
        {
            err = i2c_cb_wait();
        }
        assert(FSP_SUCCESS == err);
    }

    if(TOUCH_STATE_PRESSED == data->state)
    {
        data->point.x = coordinates[0].x;
        data->point.y = coordinates[0].y;
    }
}

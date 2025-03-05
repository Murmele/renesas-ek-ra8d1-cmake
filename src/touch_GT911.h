#ifndef __TOUCH_GT911_H
#define __TOUCH_GT911_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct{
    int32_t x;
    int32_t y;
}TouchCordinate_t;

#define GT911_REG_PRODUCT_ID       0x8140
#define GT911_REG_READ_COORD_ADDR  0x814E
#define GT911_REG_POINT1_X_ADDR    0x814F
#define GT911_REG_COMMAND          0x8040
#define GT911_REG_CONFIG_VERSION   0x8047
#define GT911_REG_CONFIG_CHECKSUM  0x80FF
#define GT911_REG_CONFIG_FRESH     0x8100
#define GT911_REG_FW_VER_HIGH      0x8145

//Reg 0x814E bit fields
#define BUFFER_READY           (1<< 7)
#define NUM_TOUCH_POINTS_MASK  0x0F

fsp_err_t enable_ts(rm_comms_ctrl_t * p_api_i2c_ctrl);
fsp_err_t init_ts(rm_comms_ctrl_t * p_api_ctrl);
fsp_err_t i2c_cb_wait(void);

#if defined(__cplusplus)
}
#endif

#endif

/* generated thread header file - do not edit */
#ifndef NEW_THREAD0_H_
#define NEW_THREAD0_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void new_thread0_entry(void * pvParameters);
                #else
extern void new_thread0_entry(void *pvParameters);
#endif
#include "rm_comms_i2c.h"
#include "rm_comms_api.h"
#include "r_ospi_b.h"
#include "r_spi_flash_api.h"
FSP_HEADER
/* I2C Communication Device */
extern const rm_comms_instance_t g_comms_i2c_device0;
extern rm_comms_i2c_instance_ctrl_t g_comms_i2c_device0_ctrl;
extern const rm_comms_cfg_t g_comms_i2c_device0_cfg;
#ifndef comms_i2c_callback
void comms_i2c_callback(rm_comms_callback_args_t *p_args);
#endif
#if OSPI_B_CFG_DMAC_SUPPORT_ENABLE
              #include "r_dmac.h"
            #endif
#if OSPI_CFG_DOTF_SUPPORT_ENABLE
              #include "r_sce_if.h"
            #endif
extern const spi_flash_instance_t g_ospi_flash;
extern ospi_b_instance_ctrl_t g_ospi_flash_ctrl;
extern const spi_flash_cfg_t g_ospi_flash_cfg;
FSP_FOOTER
#endif /* NEW_THREAD0_H_ */

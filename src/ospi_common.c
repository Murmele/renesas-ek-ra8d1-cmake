#include "new_thread0.h"
#include "ospi_common.h"

void ospi_reset(void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, OSPI_RES, BSP_IO_LEVEL_HIGH);
    vTaskDelay (10);
    R_IOPORT_PinWrite(&g_ioport_ctrl, OSPI_RES, BSP_IO_LEVEL_LOW);
    vTaskDelay (10);
    R_IOPORT_PinWrite(&g_ioport_ctrl, OSPI_RES, BSP_IO_LEVEL_HIGH);
    vTaskDelay (10);
}

void oclk_change(bsp_clocks_octaclk_div_t divider)
{
    /* Now update the octaclk divider. */
    bsp_octaclk_settings_t octaclk_settings;
    octaclk_settings.source_clock = BSP_CLOCKS_SOURCE_CLOCK_PLL2P;
    octaclk_settings.divider      = divider;
    R_BSP_OctaclkUpdate(&octaclk_settings);
}

void ospi_change_to_max_speed(void)
{
    fsp_err_t err;

    /* Change clock speed. */
    oclk_change(BSP_CLOCKS_OCTACLK_DIV_1);

    /* auto-calibrate after changing the speed of OM_SCLK */
//    err = R_OSPI_B_AutoCalibrate(&g_ospi_ram_ctrl);
//    assert(FSP_SUCCESS == err);

    /* auto-calibrate after changing the speed of OM_SCLK */
    err = R_OSPI_B_AutoCalibrate(&g_ospi_flash_ctrl);
    assert(FSP_SUCCESS == err);

    if (1 == R_XSPI->INTS_b.DSTOCS1)
    {
        R_XSPI->INTC_b.DSTOCS1C = 1;
    }

    R_XSPI->INTE_b.DSTOCS0E = 1;
    R_XSPI->INTE_b.BUSERRCH0E = 1;
    R_XSPI->INTE_b.BUSERRCH1E = 1;

    R_BSP_IrqCfgEnable(VECTOR_NUMBER_XSPI_ERR, 12, NULL);

}
